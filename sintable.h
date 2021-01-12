#ifndef sintable
#define sintable

#include "MKL05Z4.h"
static const int16_t sin_tabl = 1000;
static const int16_t sin_tab[sin_tabl]={2047,2050,2053,2057,2060,2063,2066,2070,2073,2076,2079,2082,2086,2089,2092,2095,2099,2102,2105,2108,2111,2115,2118,2121,2124,2127,2131,2134,2137,2140,2144,2147,2150,2153,2156,2160,2163,2166,2169,2172,2176,2179,2182,2185,2189,2192,2195,2198,2201,2205,2208,2211,2214,2217,2221,2224,2227,2230,2233,2237,2240,2243,2246,2249,2253,2256,2259,2262,2266,2269,2272,2275,2278,2282,2285,2288,2291,2294,2297,2301,2304,2307,2310,2313,2317,2320,2323,2326,2329,2333,2336,2339,2342,2345,2349,2352,2355,2358,2361,2364,2368,2371,2374,2377,2380,2384,2387,2390,2393,2396,2399,2403,2406,2409,2412,2415,2418,2422,2425,2428,2431,2434,2437,2441,2444,2447,2450,2453,2456,2459,2463,2466,2469,2472,2475,2478,2482,2485,2488,2491,2494,2497,2500,2504,2507,2510,2513,2516,2519,2522,2525,2529,2532,2535,2538,2541,2544,2547,2550,2554,2557,2560,2563,2566,2569,2572,2575,2578,2582,2585,2588,2591,2594,2597,2600,2603,2606,2610,2613,2616,2619,2622,2625,2628,2631,2634,2637,2640,2643,2647,2650,2653,2656,2659,2662,2665,2668,2671,2674,2677,2680,2683,2686,2690,2693,2696,2699,2702,2705,2708,2711,2714,2717,2720,2723,2726,2729,2732,2735,2738,2741,2744,2747,2750,2753,2756,2759,2762,2765,2768,2771,2774,2777,2780,2783,2786,2789,2792,2795,2798,2801,2804,2807,2810,2813,2816,2819,2822,2825,2828,2831,2834,2837,2840,2843,2846,2849,2852,2855,2858,2861,2864,2867,2870,2873,2876,2879,2882,2884,2887,2890,2893,2896,2899,2902,2905,2908,2911,2914,2917,2920,2923,2925,2928,2931,2934,2937,2940,2943,2946,2949,2952,2954,2957,2960,2963,2966,2969,2972,2975,2977,2980,2983,2986,2989,2992,2995,2997,3000,3003,3006,3009,3012,3014,3017,3020,3023,3026,3029,3031,3034,3037,3040,3043,3046,3048,3051,3054,3057,3060,3062,3065,3068,3071,3074,3076,3079,3082,3085,3087,3090,3093,3096,3099,3101,3104,3107,3110,3112,3115,3118,3121,3123,3126,3129,3131,3134,3137,3140,3142,3145,3148,3151,3153,3156,3159,3161,3164,3167,3169,3172,3175,3177,3180,3183,3186,3188,3191,3194,3196,3199,3202,3204,3207,3210,3212,3215,3217,3220,3223,3225,3228,3231,3233,3236,3238,3241,3244,3246,3249,3252,3254,3257,3259,3262,3265,3267,3270,3272,3275,3277,3280,3283,3285,3288,3290,3293,3295,3298,3300,3303,3306,3308,3311,3313,3316,3318,3321,3323,3326,3328,3331,3333,3336,3338,3341,3343,3346,3348,3351,3353,3356,3358,3361,3363,3366,3368,3370,3373,3375,3378,3380,3383,3385,3388,3390,3392,3395,3397,3400,3402,3405,3407,3409,3412,3414,3417,3419,3421,3424,3426,3428,3431,3433,3436,3438,3440,3443,3445,3447,3450,3452,3454,3457,3459,3461,3464,3466,3468,3471,3473,3475,3478,3480,3482,3485,3487,3489,3491,3494,3496,3498,3500,3503,3505,3507,3510,3512,3514,3516,3519,3521,3523,3525,3527,3530,3532,3534,3536,3539,3541,3543,3545,3547,3549,3552,3554,3556,3558,3560,3563,3565,3567,3569,3571,3573,3575,3578,3580,3582,3584,3586,3588,3590,3592,3595,3597,3599,3601,3603,3605,3607,3609,3611,3613,3615,3618,3620,3622,3624,3626,3628,3630,3632,3634,3636,3638,3640,3642,3644,3646,3648,3650,3652,3654,3656,3658,3660,3662,3664,3666,3668,3670,3672,3674,3676,3678,3680,3682,3684,3685,3687,3689,3691,3693,3695,3697,3699,3701,3703,3705,3706,3708,3710,3712,3714,3716,3718,3720,3721,3723,3725,3727,3729,3731,3732,3734,3736,3738,3740,3742,3743,3745,3747,3749,3751,3752,3754,3756,3758,3759,3761,3763,3765,3766,3768,3770,3772,3773,3775,3777,3779,3780,3782,3784,3785,3787,3789,3791,3792,3794,3796,3797,3799,3801,3802,3804,3806,3807,3809,3810,3812,3814,3815,3817,3819,3820,3822,3823,3825,3827,3828,3830,3831,3833,3834,3836,3838,3839,3841,3842,3844,3845,3847,3848,3850,3851,3853,3855,3856,3858,3859,3861,3862,3864,3865,3866,3868,3869,3871,3872,3874,3875,3877,3878,3880,3881,3882,3884,3885,3887,3888,3890,3891,3892,3894,3895,3896,3898,3899,3901,3902,3903,3905,3906,3907,3909,3910,3911,3913,3914,3915,3917,3918,3919,3921,3922,3923,3924,3926,3927,3928,3930,3931,3932,3933,3935,3936,3937,3938,3940,3941,3942,3943,3944,3946,3947,3948,3949,3950,3952,3953,3954,3955,3956,3957,3959,3960,3961,3962,3963,3964,3965,3967,3968,3969,3970,3971,3972,3973,3974,3975,3976,3978,3979,3980,3981,3982,3983,3984,3985,3986,3987,3988,3989,3990,3991,3992,3993,3994,3995,3996,3997,3998,3999,4000,4001,4002,4003,4004,4005,4006,4007,4008,4008,4009,4010,4011,4012,4013,4014,4015,4016,4017,4017,4018,4019,4020,4021,4022,4023,4023,4024,4025,4026,4027,4028,4028,4029,4030,4031,4032,4032,4033,4034,4035,4036,4036,4037,4038,4039,4039,4040,4041,4042,4042,4043,4044,4044,4045,4046,4046,4047,4048,4049,4049,4050,4051,4051,4052,4053,4053,4054,4054,4055,4056,4056,4057,4058,4058,4059,4059,4060,4061,4061,4062,4062,4063,4063,4064,4065,4065,4066,4066,4067,4067,4068,4068,4069,4069,4070,4070,4071,4071,4072,4072,4073,4073,4074,4074,4074,4075,4075,4076,4076,4077,4077,4078,4078,4078,4079,4079,4080,4080,4080,4081,4081,4081,4082,4082,4082,4083,4083,4083,4084,4084,4084,4085,4085,4085,4086,4086,4086,4087,4087,4087,4087,4088,4088,4088,4088,4089,4089,4089,4089,4090,4090,4090,4090,4090,4091,4091,4091,4091,4091,4092,4092,4092,4092,4092,4092,4093,4093,4093,4093,4093,4093,4093,4093,4093,4094,4094,4094,4094,4094,4094,4094,4094,4094,4094,4094,4094,4094,4094,4094,4094,4094,4094,4094,4095};
#endif
