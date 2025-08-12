use crate::color_utils::{
    argb_from_linrgb, argb_from_lstar, blue_from_argb, delinearized_double, green_from_argb,
    lab_f, linearized, red_from_argb, y_from_lstar, Y_FROM_LINRGB,
};
use crate::math_utils::{atan2d, cos, hypotd, lerp, powf, sanitize_degrees, sin};
use core::f64::consts::TAU;
use core::f64::math::{ceil, floor, sqrt};

pub struct HctSolver;

impl HctSolver {
    pub fn solve_to_int(hue: f64, chroma: f64, tone: f64) -> u32 {
        if chroma < 0.0001 || !(0.0001..=99.9999).contains(&tone) {
            return argb_from_lstar(tone);
        }
        let hue_degrees = sanitize_degrees(hue);
        let hue_radians = hue_degrees.to_radians();
        let y = y_from_lstar(tone);
        if let Some(exact_answer) = find_result_by_j(hue_radians, chroma, y) {
            return exact_answer;
        }
        let linrgb = bisect_to_limit(y, hue_radians);
        argb_from_linrgb(linrgb)
    }

    pub fn solve_from_int(argb: u32) -> [f64; 3] {
        let linrgb = [
            linearized(red_from_argb(argb)),
            linearized(green_from_argb(argb)),
            linearized(blue_from_argb(argb)),
        ];

        let matrix = SCALED_DISCOUNT_FROM_LINRGB;
        let r_d = linrgb[0] * matrix[0][0] + linrgb[1] * matrix[0][1] + linrgb[2] * matrix[0][2];
        let g_d = linrgb[0] * matrix[1][0] + linrgb[1] * matrix[1][1] + linrgb[2] * matrix[1][2];
        let b_d = linrgb[0] * matrix[2][0] + linrgb[1] * matrix[2][1] + linrgb[2] * matrix[2][2];
        debug_assert!(r_d >= 0.0 && g_d >= 0.0 && b_d >= 0.0);

        let r_af = powf(r_d, 0.42);
        let g_af = powf(g_d, 0.42);
        let b_af = powf(b_d, 0.42);
        let r_a = (400.0 * r_af) / (r_af + 27.13);
        let g_a = (400.0 * g_af) / (g_af + 27.13);
        let b_a = (400.0 * b_af) / (b_af + 27.13);

        let a = (11.0 * r_a - 12.0 * g_a + b_a) / 11.0;
        let b = (r_a + g_a - 2.0 * b_a) / 9.0;

        let u = (20.0 * r_a + 20.0 * g_a + 21.0 * b_a) / 20.0;
        let p2 = (40.0 * r_a + 20.0 * g_a + b_a) / 20.0;

        let hue = sanitize_degrees(atan2d(b, a).to_degrees());

        let j = 100.0 * powf(p2 * COND_NBB_DIV_AW, COND_C_MUL_Z);
        let hue_prime = if hue < 20.14 { hue + 360.0 } else { hue };
        let e_hue = 0.25 * (cos(hue_prime.to_radians() + 2.0) + 3.8);
        let p1 = e_hue * COND_E_HUE_COEFF;
        let t = p1 * hypotd(a, b) / (u + 0.305);
        let alpha = COND_ALPHA_COEFF * powf(t, 0.9);
        let chroma = alpha * sqrt(j / 100.0);

        let y = Y_FROM_LINRGB[0] * linrgb[0]
            + Y_FROM_LINRGB[1] * linrgb[1]
            + Y_FROM_LINRGB[2] * linrgb[2];
        let tone = 116.0 * lab_f(y / 100.0) - 16.0;

        [hue, chroma, tone]
    }
}

const COND_C_MUL_Z: f64 = 1.3173270022537198;
const COND_NBB_DIV_AW: f64 = 0.03391879108791669;
const COND_ALPHA_COEFF: f64 = 0.8834525670408592;
const COND_E_HUE_COEFF: f64 = 3911.227617099521;

const SCALED_DISCOUNT_FROM_LINRGB: [[f64; 3]; 3] = [
    [
        0.001200833568784504,
        0.002389694492170889,
        0.0002795742885861124,
    ],
    [
        0.0005891086651375999,
        0.0029785502573438758,
        0.0003270666104008398,
    ],
    [
        0.00010146692491640572,
        0.0005364214359186694,
        0.0032979401770712076,
    ],
];

const LINRGB_FROM_SCALED_DISCOUNT: [[f64; 3]; 3] = [
    [1373.2198709594231, -1100.4251190754821, -7.278681089101213],
    [-271.815969077903, 559.6580465940733, -32.46047482791194],
    [1.9622899599665666, -57.173814538844006, 308.7233197812385],
];

static CRITICAL_PLANES: [f64; 255] = [
    0.015176349177441876,
    0.045529047532325624,
    0.07588174588720938,
    0.10623444424209313,
    0.13658714259697685,
    0.16693984095186062,
    0.19729253930674434,
    0.2276452376616281,
    0.2579979360165119,
    0.28835063437139563,
    0.3188300904430532,
    0.350925934958123,
    0.3848314933096426,
    0.42057480301049466,
    0.458183274052838,
    0.4976837250274023,
    0.5391024159806381,
    0.5824650784040898,
    0.6277969426914107,
    0.6751227633498623,
    0.7244668422128921,
    0.775853049866786,
    0.829304845476233,
    0.8848452951698498,
    0.942497089126609,
    1.0022825574869039,
    1.0642236851973577,
    1.1283421258858297,
    1.1946592148522128,
    1.2631959812511864,
    1.3339731595349034,
    1.407011200216447,
    1.4823302800086415,
    1.5599503113873272,
    1.6398909516233677,
    1.7221716113234105,
    1.8068114625156377,
    1.8938294463134073,
    1.9832442801866852,
    2.075074464868551,
    2.1693382909216234,
    2.2660538449872063,
    2.36523901573795,
    2.4669114995532007,
    2.5710888059345764,
    2.6777882626779785,
    2.7870270208169257,
    2.898822059350997,
    3.0131901897720907,
    3.1301480604002863,
    3.2497121605402226,
    3.3718988244681087,
    3.4967242352587946,
    3.624204428461639,
    3.754355295633311,
    3.887192587735158,
    4.022731918402185,
    4.160988767090289,
    4.301978482107941,
    4.445716283538092,
    4.592217266055746,
    4.741496401646282,
    4.893568542229298,
    5.048448422192488,
    5.20615066083972,
    5.3666897647573375,
    5.5300801301023865,
    5.696336044816294,
    5.865471690767354,
    6.037501145825082,
    6.212438385869475,
    6.390297286737924,
    6.571091626112461,
    6.7548350853498045,
    6.941541251256611,
    7.131223617812143,
    7.323895587840543,
    7.5195704746346665,
    7.7182615035334345,
    7.919981813454504,
    8.124744458384042,
    8.332562408825165,
    8.543448553206703,
    8.757415699253682,
    8.974476575321063,
    9.194643831691977,
    9.417930041841839,
    9.644347703669503,
    9.873909240696694,
    10.106627003236781,
    10.342513269534024,
    10.58158024687427,
    10.8238400726681,
    11.069304815507364,
    11.317986476196008,
    11.569896988756009,
    11.825048221409341,
    12.083451977536606,
    12.345119996613247,
    12.610063955123938,
    12.878295467455942,
    13.149826086772048,
    13.42466730586372,
    13.702830557985108,
    13.984327217668513,
    14.269168601521828,
    14.55736596900856,
    14.848930523210871,
    15.143873411576273,
    15.44220572664832,
    15.743938506781891,
    16.04908273684337,
    16.35764934889634,
    16.66964922287304,
    16.985093187232053,
    17.30399201960269,
    17.62635644741625,
    17.95219714852476,
    18.281524751807332,
    18.614349837764564,
    18.95068293910138,
    19.290534541298456,
    19.633915083172692,
    19.98083495742689,
    20.331304511189067,
    20.685334046541502,
    21.042933821039977,
    21.404114048223256,
    21.76888489811322,
    22.137256497705877,
    22.50923893145328,
    22.884842241736916,
    23.264076429332462,
    23.6469514538663,
    24.033477234264016,
    24.42366364919083,
    24.817520537484558,
    25.21505769858089,
    25.61628489293138,
    26.021211842414342,
    26.429848230738664,
    26.842203703840827,
    27.258287870275353,
    27.678110301598522,
    28.10168053274597,
    28.529008062403893,
    28.96010235337422,
    29.39497283293396,
    29.83362889318845,
    30.276079891419332,
    30.722335150426627,
    31.172403958865512,
    31.62629557157785,
    32.08401920991837,
    32.54558406207592,
    33.010999283389665,
    33.4802739966603,
    33.953417292456834,
    34.430438229418264,
    34.911345834551085,
    35.39614910352207,
    35.88485700094671,
    36.37747846067349,
    36.87402238606382,
    37.37449765026789,
    37.87891309649659,
    38.38727753828926,
    38.89959975977785,
    39.41588851594697,
    39.93615253289054,
    40.460400508064545,
    40.98864111053629,
    41.520882981230194,
    42.05713473317016,
    42.597404951718396,
    43.141702194811224,
    43.6900349931913,
    44.24241185063697,
    44.798841244188324,
    45.35933162437017,
    45.92389141541209,
    46.49252901546552,
    47.065252796817916,
    47.64207110610409,
    48.22299226451468,
    48.808024568002054,
    49.3971762874833,
    49.9904556690408,
    50.587870934119984,
    51.189430279724725,
    51.79514187861014,
    52.40501387947288,
    53.0190544071392,
    53.637271562750364,
    54.259673423945976,
    54.88626804504493,
    55.517063457223934,
    56.15206766869424,
    56.79128866487574,
    57.43473440856916,
    58.08241284012621,
    58.734331877617365,
    59.39049941699807,
    60.05092333227251,
    60.715611475655585,
    61.38457167773311,
    62.057811747619894,
    62.7353394731159,
    63.417162620860914,
    64.10328893648692,
    64.79372614476921,
    65.48848194977529,
    66.18756403501224,
    66.89098006357258,
    67.59873767827808,
    68.31084450182222,
    69.02730813691093,
    69.74813616640164,
    70.47333615344107,
    71.20291564160104,
    71.93688215501312,
    72.67524319850172,
    73.41800625771542,
    74.16517879925733,
    74.9167682708136,
    75.67278210128072,
    76.43322770089146,
    77.1981124613393,
    77.96744375590167,
    78.74122893956174,
    79.51947534912904,
    80.30219030335869,
    81.08938110306934,
    81.88105503125999,
    82.67721935322541,
    83.4778813166706,
    84.28304815182372,
    85.09272707154808,
    85.90692527145302,
    86.72564993000343,
    87.54890820862819,
    88.3767072518277,
    89.2090541872801,
    90.04595612594655,
    90.88742016217518,
    91.73345337380438,
    92.58406282226491,
    93.43925555268066,
    94.29903859396902,
    95.16341895893969,
    96.03240364439274,
    96.9059996312159,
    97.78421388448044,
    98.6670533535366,
    99.55452497210776,
];

const fn sanitize_radians(angle: f64) -> f64 {
    (angle + TAU * 4.0) % TAU
}

fn hue_of(linrgb: [f64; 3]) -> f64 {
    let matrix = SCALED_DISCOUNT_FROM_LINRGB;
    let r_d = linrgb[0] * matrix[0][0] + linrgb[1] * matrix[0][1] + linrgb[2] * matrix[0][2];
    let g_d = linrgb[0] * matrix[1][0] + linrgb[1] * matrix[1][1] + linrgb[2] * matrix[1][2];
    let b_d = linrgb[0] * matrix[2][0] + linrgb[1] * matrix[2][1] + linrgb[2] * matrix[2][2];
    debug_assert!(r_d >= 0.0 && g_d >= 0.0 && b_d >= 0.0);

    let r_af = powf(r_d, 0.42);
    let g_af = powf(g_d, 0.42);
    let b_af = powf(b_d, 0.42);
    let r_a_scaled = r_af / (r_af + 27.13);
    let g_a_scaled = g_af / (g_af + 27.13);
    let b_a_scaled = b_af / (b_af + 27.13);

    let a = (11.0 * r_a_scaled - 12.0 * g_a_scaled + b_a_scaled) / 11.0;
    let b = (r_a_scaled + g_a_scaled - 2.0 * b_a_scaled) / 9.0;

    atan2d(b, a)
}

const fn are_in_cyclic_order(a: f64, b: f64, c: f64) -> bool {
    let delta_ab = sanitize_radians(b - a);
    let delta_ac = sanitize_radians(c - a);
    delta_ab < delta_ac
}

const fn intercept(source: f64, mid: f64, target: f64) -> f64 {
    (mid - source) / (target - source)
}

const fn set_coordinate(
    source: [f64; 3],
    coordinate: f64,
    target: [f64; 3],
    axis: usize,
) -> [f64; 3] {
    let t = intercept(source[axis], coordinate, target[axis]);
    [
        lerp(source[0], target[0], t),
        lerp(source[1], target[1], t),
        lerp(source[2], target[2], t),
    ]
}

const fn is_bounded(x: f64) -> bool {
    0.0 <= x && x <= 100.0
}

const fn nth_vertex(y: f64, n: usize) -> Option<[f64; 3]> {
    let k_r = Y_FROM_LINRGB[0];
    let k_g = Y_FROM_LINRGB[1];
    let k_b = Y_FROM_LINRGB[2];
    let coord_a = if n % 4 <= 1 { 0.0 } else { 100.0 };
    let coord_b = if n % 2 == 0 { 0.0 } else { 100.0 };

    if n < 4 {
        let g = coord_a;
        let b = coord_b;
        let r = (y - g * k_g - b * k_b) / k_r;
        if is_bounded(r) { Some([r, g, b]) } else { None }
    } else if n < 8 {
        let b = coord_a;
        let r = coord_b;
        let g = (y - r * k_r - b * k_b) / k_g;
        if is_bounded(g) { Some([r, g, b]) } else { None }
    } else if n < 12 {
        let r = coord_a;
        let g = coord_b;
        let b = (y - r * k_r - g * k_g) / k_b;
        if is_bounded(b) { Some([r, g, b]) } else { None }
    } else {
        unreachable!();
    }
}

fn bisect_to_segment(y: f64, target_hue: f64) -> [[f64; 3]; 2] {
    let mut left = [0.0; 3];
    let mut right = [0.0; 3];
    let mut left_hue = 0.0;
    let mut right_hue = 0.0;
    let mut initialized = false;
    let mut uncut = true;

    for n in 0..12 {
        let Some(mid) = nth_vertex(y, n) else {
            continue;
        };
        let mid_hue = hue_of(mid);
        if !initialized {
            left = mid;
            right = mid;
            left_hue = mid_hue;
            right_hue = mid_hue;
            initialized = true;
            continue;
        }
        if uncut || are_in_cyclic_order(left_hue, mid_hue, right_hue) {
            uncut = false;
            if are_in_cyclic_order(left_hue, target_hue, mid_hue) {
                right = mid;
                right_hue = mid_hue;
            } else {
                left = mid;
                left_hue = mid_hue;
            }
        }
    }

    [left, right]
}

const fn midpoint(a: [f64; 3], b: [f64; 3]) -> [f64; 3] {
    [
        (a[0] + b[0]) / 2.0,
        (a[1] + b[1]) / 2.0,
        (a[2] + b[2]) / 2.0,
    ]
}

const fn critical_plane_below(x: f64) -> i16 {
    floor(x - 0.5) as i16
}

const fn critical_plane_above(x: f64) -> i16 {
    ceil(x - 0.5) as i16
}

fn bisect_to_limit(y: f64, target_hue: f64) -> [f64; 3] {
    let [mut left, mut right] = bisect_to_segment(y, target_hue);
    let mut left_hue = hue_of(left);

    for axis in 0..3 {
        if left[axis] != right[axis] {
            let mut l_plane;
            let mut r_plane;

            if left[axis] < right[axis] {
                l_plane = critical_plane_below(delinearized_double(left[axis]));
                r_plane = critical_plane_above(delinearized_double(right[axis]));
            } else {
                l_plane = critical_plane_above(delinearized_double(left[axis]));
                r_plane = critical_plane_below(delinearized_double(right[axis]));
            }

            for _ in 0..8 {
                if (r_plane - l_plane).abs() <= 1 {
                    break;
                } else {
                    let m_plane = (l_plane + r_plane) / 2;
                    let mid_plane_coordinate = CRITICAL_PLANES[m_plane as usize];
                    let mid = set_coordinate(left, mid_plane_coordinate, right, axis);
                    let mid_hue = hue_of(mid);

                    if are_in_cyclic_order(left_hue, target_hue, mid_hue) {
                        right = mid;
                        r_plane = m_plane;
                    } else {
                        left = mid;
                        left_hue = mid_hue;
                        l_plane = m_plane;
                    }
                }
            }
        }
    }

    midpoint(left, right)
}

fn inverse_chromatic_adaptation(adapted: f64) -> f64 {
    let adapted_abs = adapted.abs();
    let base = (27.13 * adapted_abs / (400.0 - adapted_abs)).max(0.0);
    adapted.signum() * powf(base, 1.0 / 0.42)
}

fn find_result_by_j(hue_radians: f64, chroma: f64, y: f64) -> Option<u32> {
    let mut j = sqrt(y) * 11.0;
    let e_hue = 0.25 * (cos(hue_radians + 2.0) + 3.8);
    let p1 = e_hue * COND_E_HUE_COEFF;
    let h_sin = sin(hue_radians);
    let h_cos = cos(hue_radians);

    for i in 0..5 {
        let j_normalized = j / 100.0;
        let alpha = if chroma == 0.0 || j == 0.0 {
            0.0
        } else {
            chroma / sqrt(j_normalized)
        };
        let t = powf(alpha / COND_ALPHA_COEFF, 1.0 / 0.9);
        let p2 = powf(j_normalized, 1.0 / COND_C_MUL_Z) / COND_NBB_DIV_AW;
        let gamma = (23.0 * (p2 + 0.305) * t) / (23.0 * p1 + 11.0 * t * h_cos + 108.0 * t * h_sin);
        let a = gamma * h_cos;
        let b = gamma * h_sin;
        let r_a = (460.0 * p2 + 451.0 * a + 288.0 * b) / 1403.0;
        let g_a = (460.0 * p2 - 891.0 * a - 261.0 * b) / 1403.0;
        let b_a = (460.0 * p2 - 220.0 * a - 6300.0 * b) / 1403.0;
        let r_c_scaled = inverse_chromatic_adaptation(r_a);
        let g_c_scaled = inverse_chromatic_adaptation(g_a);
        let b_c_scaled = inverse_chromatic_adaptation(b_a);
        let matrix = LINRGB_FROM_SCALED_DISCOUNT;
        let linrgb = [
            r_c_scaled * matrix[0][0] + g_c_scaled * matrix[0][1] + b_c_scaled * matrix[0][2],
            r_c_scaled * matrix[1][0] + g_c_scaled * matrix[1][1] + b_c_scaled * matrix[1][2],
            r_c_scaled * matrix[2][0] + g_c_scaled * matrix[2][1] + b_c_scaled * matrix[2][2],
        ];

        if linrgb[0] < 0.0 || linrgb[1] < 0.0 || linrgb[2] < 0.0 {
            return None;
        }

        let fnj = Y_FROM_LINRGB[0] * linrgb[0]
            + Y_FROM_LINRGB[1] * linrgb[1]
            + Y_FROM_LINRGB[2] * linrgb[2];

        if fnj <= 0.0 {
            return None;
        }

        if i == 4 || (fnj - y).abs() < 0.002 {
            return if linrgb[0] > 100.01 || linrgb[1] > 100.01 || linrgb[2] > 100.01 {
                None
            } else {
                Some(argb_from_linrgb(linrgb))
            };
        }

        j = j - (fnj - y) * j / (2.0 * fnj);
    }

    unreachable!();
}
