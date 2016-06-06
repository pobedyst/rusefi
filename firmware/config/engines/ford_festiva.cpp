/*
 * @file ford_festiva.cpp
 *
 * set_engine_type 14
 *
 * http://rusefi.com/wiki/index.php?title=Vehicle:Mazda_Protege_1993
 * http://rusefi.com/forum/viewtopic.php?f=3&t=537
 * http://rusefi.com/forum/viewtopic.php?f=3&t=822
 *
 * @date Jan 3, 2016
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "ford_festiva.h"
#include "thermistors.h"
#include "engine_math.h"
#include "maf.h"
#include "ego.h"
#include "fsio_impl.h"
#include "mazda_miata.h"
#include "custom_engine.h"

static const ignition_table_t racingFestivaIgnitionTable = {
		/* Generated by TS2C on Mon Feb 08 13:59:57 EST 2016*/
		{/* 0 10.000	*//* 0 800.0*/13.000,	/* 1 1213.3281*/12.900,	/* 2 1626.6562*/10.500,	/* 3 2040.0*/10.500,	/* 4 2453.3438*/10.500,	/* 5 2866.6562*/10.500,	/* 6 3280.0*/11.500,	/* 7 3693.3438*/11.500,	/* 8 4106.6875*/11.500,	/* 9 4520.0*/12.000,	/* 10 4933.3125*/12.000,	/* 11 5346.6875*/12.000,	/* 12 5760.0*/12.000,	/* 13 6173.3125*/12.000,	/* 14 6586.6875*/12.000,	/* 15 7000.0*/12.000,	},
		{/* 1 16.000	*//* 0 800.0*/13.000,	/* 1 1213.3281*/12.900,	/* 2 1626.6562*/10.500,	/* 3 2040.0*/10.500,	/* 4 2453.3438*/10.500,	/* 5 2866.6562*/10.500,	/* 6 3280.0*/11.500,	/* 7 3693.3438*/11.500,	/* 8 4106.6875*/11.500,	/* 9 4520.0*/12.000,	/* 10 4933.3125*/12.000,	/* 11 5346.6875*/12.000,	/* 12 5760.0*/12.000,	/* 13 6173.3125*/12.000,	/* 14 6586.6875*/12.000,	/* 15 7000.0*/12.000,	},
		{/* 2 22.000	*//* 0 800.0*/14.600,	/* 1 1213.3281*/14.000,	/* 2 1626.6562*/14.900,	/* 3 2040.0*/12.400,	/* 4 2453.3438*/12.300,	/* 5 2866.6562*/12.300,	/* 6 3280.0*/12.200,	/* 7 3693.3438*/12.200,	/* 8 4106.6875*/12.100,	/* 9 4520.0*/12.000,	/* 10 4933.3125*/12.000,	/* 11 5346.6875*/12.000,	/* 12 5760.0*/12.000,	/* 13 6173.3125*/12.000,	/* 14 6586.6875*/12.000,	/* 15 7000.0*/12.000,	},
		{/* 3 28.000	*//* 0 800.0*/14.600,	/* 1 1213.3281*/14.000,	/* 2 1626.6562*/14.900,	/* 3 2040.0*/12.400,	/* 4 2453.3438*/12.300,	/* 5 2866.6562*/12.300,	/* 6 3280.0*/12.200,	/* 7 3693.3438*/12.200,	/* 8 4106.6875*/12.100,	/* 9 4520.0*/12.000,	/* 10 4933.3125*/12.000,	/* 11 5346.6875*/12.000,	/* 12 5760.0*/12.000,	/* 13 6173.3125*/12.000,	/* 14 6586.6875*/12.000,	/* 15 7000.0*/12.000,	},
		{/* 4 31.000	*//* 0 800.0*/15.200,	/* 1 1213.3281*/15.400,	/* 2 1626.6562*/15.400,	/* 3 2040.0*/15.400,	/* 4 2453.3438*/15.400,	/* 5 2866.6562*/16.800,	/* 6 3280.0*/16.800,	/* 7 3693.3438*/23.000,	/* 8 4106.6875*/23.000,	/* 9 4520.0*/26.000,	/* 10 4933.3125*/26.000,	/* 11 5346.6875*/26.000,	/* 12 5760.0*/26.000,	/* 13 6173.3125*/26.000,	/* 14 6586.6875*/26.000,	/* 15 7000.0*/26.000,	},
		{/* 5 35.000	*//* 0 800.0*/15.200,	/* 1 1213.3281*/15.400,	/* 2 1626.6562*/15.400,	/* 3 2040.0*/15.400,	/* 4 2453.3438*/15.400,	/* 5 2866.6562*/16.800,	/* 6 3280.0*/16.800,	/* 7 3693.3438*/23.000,	/* 8 4106.6875*/23.000,	/* 9 4520.0*/26.000,	/* 10 4933.3125*/26.000,	/* 11 5346.6875*/26.000,	/* 12 5760.0*/26.000,	/* 13 6173.3125*/26.000,	/* 14 6586.6875*/26.000,	/* 15 7000.0*/26.000,	},
		{/* 6 38.000	*//* 0 800.0*/16.700,	/* 1 1213.3281*/16.700,	/* 2 1626.6562*/17.000,	/* 3 2040.0*/19.000,	/* 4 2453.3438*/19.000,	/* 5 2866.6562*/21.500,	/* 6 3280.0*/21.500,	/* 7 3693.3438*/25.800,	/* 8 4106.6875*/25.800,	/* 9 4520.0*/26.000,	/* 10 4933.3125*/26.000,	/* 11 5346.6875*/26.000,	/* 12 5760.0*/26.000,	/* 13 6173.3125*/26.000,	/* 14 6586.6875*/26.000,	/* 15 7000.0*/26.000,	},
		{/* 7 41.000	*//* 0 800.0*/16.700,	/* 1 1213.3281*/16.700,	/* 2 1626.6562*/17.000,	/* 3 2040.0*/19.000,	/* 4 2453.3438*/19.000,	/* 5 2866.6562*/21.500,	/* 6 3280.0*/21.500,	/* 7 3693.3438*/25.800,	/* 8 4106.6875*/25.800,	/* 9 4520.0*/26.000,	/* 10 4933.3125*/26.000,	/* 11 5346.6875*/26.000,	/* 12 5760.0*/26.000,	/* 13 6173.3125*/26.000,	/* 14 6586.6875*/26.000,	/* 15 7000.0*/26.000,	},
		{/* 8 48.000	*//* 0 800.0*/16.900,	/* 1 1213.3281*/17.000,	/* 2 1626.6562*/17.000,	/* 3 2040.0*/19.700,	/* 4 2453.3438*/19.700,	/* 5 2866.6562*/22.200,	/* 6 3280.0*/22.500,	/* 7 3693.3438*/22.500,	/* 8 4106.6875*/30.000,	/* 9 4520.0*/30.000,	/* 10 4933.3125*/30.000,	/* 11 5346.6875*/30.000,	/* 12 5760.0*/30.000,	/* 13 6173.3125*/30.000,	/* 14 6586.6875*/30.000,	/* 15 7000.0*/30.000,	},
		{/* 9 52.000	*//* 0 800.0*/17.000,	/* 1 1213.3281*/17.100,	/* 2 1626.6562*/17.000,	/* 3 2040.0*/20.100,	/* 4 2453.3438*/20.100,	/* 5 2866.6562*/22.600,	/* 6 3280.0*/23.000,	/* 7 3693.3438*/23.000,	/* 8 4106.6875*/30.000,	/* 9 4520.0*/30.000,	/* 10 4933.3125*/30.000,	/* 11 5346.6875*/30.000,	/* 12 5760.0*/30.000,	/* 13 6173.3125*/30.000,	/* 14 6586.6875*/30.000,	/* 15 7000.0*/30.000,	},
		{/* 10 59.000	*//* 0 800.0*/17.200,	/* 1 1213.3281*/17.400,	/* 2 1626.6562*/17.000,	/* 3 2040.0*/21.000,	/* 4 2453.3438*/21.000,	/* 5 2866.6562*/23.400,	/* 6 3280.0*/23.900,	/* 7 3693.3438*/23.900,	/* 8 4106.6875*/30.000,	/* 9 4520.0*/30.000,	/* 10 4933.3125*/30.000,	/* 11 5346.6875*/30.000,	/* 12 5760.0*/30.000,	/* 13 6173.3125*/30.000,	/* 14 6586.6875*/30.000,	/* 15 7000.0*/30.000,	},
		{/* 11 65.000	*//* 0 800.0*/16.200,	/* 1 1213.3281*/16.400,	/* 2 1626.6562*/16.400,	/* 3 2040.0*/21.000,	/* 4 2453.3438*/21.000,	/* 5 2866.6562*/23.800,	/* 6 3280.0*/24.000,	/* 7 3693.3438*/24.000,	/* 8 4106.6875*/30.000,	/* 9 4520.0*/30.000,	/* 10 4933.3125*/30.000,	/* 11 5346.6875*/30.000,	/* 12 5760.0*/30.000,	/* 13 6173.3125*/30.000,	/* 14 6586.6875*/30.000,	/* 15 7000.0*/30.000,	},
		{/* 12 74.000	*//* 0 800.0*/15.300,	/* 1 1213.3281*/15.900,	/* 2 1626.6562*/15.900,	/* 3 2040.0*/19.500,	/* 4 2453.3438*/19.100,	/* 5 2866.6562*/23.100,	/* 6 3280.0*/23.900,	/* 7 3693.3438*/23.900,	/* 8 4106.6875*/29.200,	/* 9 4520.0*/28.600,	/* 10 4933.3125*/30.000,	/* 11 5346.6875*/30.000,	/* 12 5760.0*/30.000,	/* 13 6173.3125*/30.000,	/* 14 6586.6875*/30.000,	/* 15 7000.0*/30.000,	},
		{/* 13 85.000	*//* 0 800.0*/20.000,	/* 1 1213.3281*/15.900,	/* 2 1626.6562*/15.900,	/* 3 2040.0*/19.500,	/* 4 2453.3438*/19.500,	/* 5 2866.6562*/24.800,	/* 6 3280.0*/26.000,	/* 7 3693.3438*/26.000,	/* 8 4106.6875*/26.900,	/* 9 4520.0*/27.500,	/* 10 4933.3125*/27.600,	/* 11 5346.6875*/28.100,	/* 12 5760.0*/28.000,	/* 13 6173.3125*/28.700,	/* 14 6586.6875*/28.500,	/* 15 7000.0*/30.000,	},
		{/* 14 94.000	*//* 0 800.0*/20.000,	/* 1 1213.3281*/16.000,	/* 2 1626.6562*/16.000,	/* 3 2040.0*/19.500,	/* 4 2453.3438*/19.700,	/* 5 2866.6562*/25.000,	/* 6 3280.0*/26.200,	/* 7 3693.3438*/26.200,	/* 8 4106.6875*/26.900,	/* 9 4520.0*/27.500,	/* 10 4933.3125*/27.600,	/* 11 5346.6875*/28.100,	/* 12 5760.0*/28.000,	/* 13 6173.3125*/28.700,	/* 14 6586.6875*/28.900,	/* 15 7000.0*/30.200,	},
		{/* 15 101.000	*//* 0 800.0*/20.000,	/* 1 1213.3281*/16.200,	/* 2 1626.6562*/16.200,	/* 3 2040.0*/19.500,	/* 4 2453.3438*/19.500,	/* 5 2866.6562*/23.100,	/* 6 3280.0*/24.000,	/* 7 3693.3438*/24.000,	/* 8 4106.6875*/26.900,	/* 9 4520.0*/27.500,	/* 10 4933.3125*/27.600,	/* 11 5346.6875*/28.100,	/* 12 5760.0*/28.000,	/* 13 6173.3125*/30.000,	/* 14 6586.6875*/30.900,	/* 15 7000.0*/30.200,	},
};

static const fuel_table_t racingFestivaVeTable = {
		/* Generated by TS2C on Mon Feb 08 13:58:49 EST 2016*/
		{/* 0 10.000	*//* 0 800.0*/110.000,	/* 1 1200.0*/105.384,	/* 2 1600.0*/94.188,	/* 3 2000.0*/104.591,	/* 4 2400.0*/108.557,	/* 5 2800.0*/102.816,	/* 6 3200.0*/104.492,	/* 7 3600.0*/106.760,	/* 8 4100.0*/130.328,	/* 9 4500.0*/130.600,	/* 10 4900.0*/110.748,	/* 11 5300.0*/110.622,	/* 12 5700.0*/110.000,	/* 13 6100.0*/110.000,	/* 14 6500.0*/110.000,	/* 15 7000.0*/110.000,	},
		{/* 1 20.000	*//* 0 800.0*/92.912,	/* 1 1200.0*/88.219,	/* 2 1600.0*/87.050,	/* 3 2000.0*/94.294,	/* 4 2400.0*/100.350,	/* 5 2800.0*/101.178,	/* 6 3200.0*/99.942,	/* 7 3600.0*/108.697,	/* 8 4100.0*/110.794,	/* 9 4500.0*/107.359,	/* 10 4900.0*/107.412,	/* 11 5300.0*/110.000,	/* 12 5700.0*/110.000,	/* 13 6100.0*/110.000,	/* 14 6500.0*/110.000,	/* 15 7000.0*/110.000,	},
		{/* 2 30.000	*//* 0 800.0*/93.691,	/* 1 1200.0*/90.029,	/* 2 1600.0*/94.296,	/* 3 2000.0*/98.011,	/* 4 2400.0*/104.727,	/* 5 2800.0*/111.653,	/* 6 3200.0*/113.626,	/* 7 3600.0*/117.779,	/* 8 4100.0*/119.690,	/* 9 4500.0*/118.623,	/* 10 4900.0*/114.264,	/* 11 5300.0*/115.000,	/* 12 5700.0*/115.000,	/* 13 6100.0*/115.000,	/* 14 6500.0*/115.000,	/* 15 7000.0*/115.000,	},
		{/* 3 40.000	*//* 0 800.0*/99.005,	/* 1 1200.0*/100.483,	/* 2 1600.0*/102.497,	/* 3 2000.0*/107.411,	/* 4 2400.0*/110.811,	/* 5 2800.0*/116.596,	/* 6 3200.0*/116.402,	/* 7 3600.0*/117.253,	/* 8 4100.0*/121.555,	/* 9 4500.0*/121.311,	/* 10 4900.0*/115.000,	/* 11 5300.0*/115.000,	/* 12 5700.0*/115.000,	/* 13 6100.0*/115.000,	/* 14 6500.0*/115.000,	/* 15 7000.0*/115.000,	},
		{/* 4 50.000	*//* 0 800.0*/106.376,	/* 1 1200.0*/112.563,	/* 2 1600.0*/103.710,	/* 3 2000.0*/106.785,	/* 4 2400.0*/111.187,	/* 5 2800.0*/121.104,	/* 6 3200.0*/120.854,	/* 7 3600.0*/122.625,	/* 8 4100.0*/125.261,	/* 9 4500.0*/120.702,	/* 10 4900.0*/115.000,	/* 11 5300.0*/115.000,	/* 12 5700.0*/115.000,	/* 13 6100.0*/115.000,	/* 14 6500.0*/115.000,	/* 15 7000.0*/115.000,	},
		{/* 5 60.000	*//* 0 800.0*/108.366,	/* 1 1200.0*/107.741,	/* 2 1600.0*/105.713,	/* 3 2000.0*/107.654,	/* 4 2400.0*/113.228,	/* 5 2800.0*/124.786,	/* 6 3200.0*/121.577,	/* 7 3600.0*/119.707,	/* 8 4100.0*/121.255,	/* 9 4500.0*/120.000,	/* 10 4900.0*/115.000,	/* 11 5300.0*/115.000,	/* 12 5700.0*/115.000,	/* 13 6100.0*/115.000,	/* 14 6500.0*/115.000,	/* 15 7000.0*/115.000,	},
		{/* 6 70.000	*//* 0 800.0*/106.030,	/* 1 1200.0*/108.577,	/* 2 1600.0*/112.935,	/* 3 2000.0*/113.187,	/* 4 2400.0*/113.910,	/* 5 2800.0*/109.719,	/* 6 3200.0*/97.090,	/* 7 3600.0*/120.000,	/* 8 4100.0*/120.000,	/* 9 4500.0*/120.000,	/* 10 4900.0*/120.000,	/* 11 5300.0*/120.000,	/* 12 5700.0*/120.000,	/* 13 6100.0*/120.000,	/* 14 6500.0*/120.000,	/* 15 7000.0*/120.000,	},
		{/* 7 80.000	*//* 0 800.0*/109.383,	/* 1 1200.0*/107.073,	/* 2 1600.0*/107.386,	/* 3 2000.0*/113.634,	/* 4 2400.0*/113.413,	/* 5 2800.0*/120.000,	/* 6 3200.0*/120.000,	/* 7 3600.0*/120.000,	/* 8 4100.0*/120.000,	/* 9 4500.0*/120.000,	/* 10 4900.0*/120.000,	/* 11 5300.0*/120.000,	/* 12 5700.0*/120.000,	/* 13 6100.0*/120.000,	/* 14 6500.0*/120.000,	/* 15 7000.0*/120.000,	},
		{/* 8 90.000	*//* 0 800.0*/107.380,	/* 1 1200.0*/109.401,	/* 2 1600.0*/109.114,	/* 3 2000.0*/115.732,	/* 4 2400.0*/110.736,	/* 5 2800.0*/120.000,	/* 6 3200.0*/120.000,	/* 7 3600.0*/120.000,	/* 8 4100.0*/120.000,	/* 9 4500.0*/120.611,	/* 10 4900.0*/120.000,	/* 11 5300.0*/120.000,	/* 12 5700.0*/120.000,	/* 13 6100.0*/120.000,	/* 14 6500.0*/120.000,	/* 15 7000.0*/120.000,	},
		{/* 9 100.000	*//* 0 800.0*/97.878,	/* 1 1200.0*/100.000,	/* 2 1600.0*/100.000,	/* 3 2000.0*/104.241,	/* 4 2400.0*/100.000,	/* 5 2800.0*/120.000,	/* 6 3200.0*/120.000,	/* 7 3600.0*/120.000,	/* 8 4100.0*/120.711,	/* 9 4500.0*/121.226,	/* 10 4900.0*/121.317,	/* 11 5300.0*/120.000,	/* 12 5700.0*/120.000,	/* 13 6100.0*/120.000,	/* 14 6500.0*/120.000,	/* 15 7000.0*/120.000,	},
		{/* 10 110.000	*//* 0 800.0*/100.000,	/* 1 1200.0*/100.000,	/* 2 1600.0*/100.000,	/* 3 2000.0*/100.000,	/* 4 2400.0*/100.000,	/* 5 2800.0*/120.000,	/* 6 3200.0*/120.000,	/* 7 3600.0*/120.000,	/* 8 4100.0*/120.000,	/* 9 4500.0*/120.000,	/* 10 4900.0*/120.000,	/* 11 5300.0*/120.000,	/* 12 5700.0*/120.000,	/* 13 6100.0*/120.000,	/* 14 6500.0*/120.000,	/* 15 7000.0*/120.000,	},
		{/* 11 120.000	*//* 0 800.0*/100.000,	/* 1 1200.0*/100.000,	/* 2 1600.0*/100.000,	/* 3 2000.0*/100.000,	/* 4 2400.0*/100.000,	/* 5 2800.0*/120.000,	/* 6 3200.0*/120.000,	/* 7 3600.0*/120.000,	/* 8 4100.0*/120.000,	/* 9 4500.0*/120.000,	/* 10 4900.0*/120.000,	/* 11 5300.0*/120.000,	/* 12 5700.0*/120.000,	/* 13 6100.0*/120.000,	/* 14 6500.0*/120.000,	/* 15 7000.0*/120.000,	},
		{/* 12 130.000	*//* 0 800.0*/100.000,	/* 1 1200.0*/100.000,	/* 2 1600.0*/100.000,	/* 3 2000.0*/100.000,	/* 4 2400.0*/100.000,	/* 5 2800.0*/120.000,	/* 6 3200.0*/120.000,	/* 7 3600.0*/120.000,	/* 8 4100.0*/120.000,	/* 9 4500.0*/120.000,	/* 10 4900.0*/120.000,	/* 11 5300.0*/120.000,	/* 12 5700.0*/120.000,	/* 13 6100.0*/120.000,	/* 14 6500.0*/120.000,	/* 15 7000.0*/120.000,	},
		{/* 13 140.000	*//* 0 800.0*/100.000,	/* 1 1200.0*/100.000,	/* 2 1600.0*/100.000,	/* 3 2000.0*/100.000,	/* 4 2400.0*/100.000,	/* 5 2800.0*/120.000,	/* 6 3200.0*/120.000,	/* 7 3600.0*/120.000,	/* 8 4100.0*/120.000,	/* 9 4500.0*/120.000,	/* 10 4900.0*/120.000,	/* 11 5300.0*/120.000,	/* 12 5700.0*/120.000,	/* 13 6100.0*/120.000,	/* 14 6500.0*/120.000,	/* 15 7000.0*/120.000,	},
		{/* 14 150.000	*//* 0 800.0*/100.000,	/* 1 1200.0*/100.000,	/* 2 1600.0*/100.000,	/* 3 2000.0*/100.000,	/* 4 2400.0*/100.000,	/* 5 2800.0*/120.000,	/* 6 3200.0*/120.000,	/* 7 3600.0*/120.000,	/* 8 4100.0*/120.000,	/* 9 4500.0*/120.000,	/* 10 4900.0*/120.000,	/* 11 5300.0*/120.000,	/* 12 5700.0*/120.000,	/* 13 6100.0*/120.000,	/* 14 6500.0*/120.000,	/* 15 7000.0*/120.000,	},
		{/* 15 160.000	*//* 0 800.0*/100.000,	/* 1 1200.0*/100.000,	/* 2 1600.0*/100.000,	/* 3 2000.0*/100.000,	/* 4 2400.0*/100.000,	/* 5 2800.0*/120.000,	/* 6 3200.0*/120.000,	/* 7 3600.0*/120.000,	/* 8 4100.0*/120.000,	/* 9 4500.0*/120.000,	/* 10 4900.0*/120.000,	/* 11 5300.0*/120.000,	/* 12 5700.0*/120.000,	/* 13 6100.0*/120.000,	/* 14 6500.0*/120.000,	/* 15 7000.0*/120.000,	},
};

EXTERN_ENGINE;

/**
 * pin 1I/W9 - extra +5v
 * set_engine_type 14
 */
void setFordEscortGt(DECLARE_ENGINE_PARAMETER_F) {
	engineConfiguration->trigger.type = TT_MAZDA_DOHC_1_4;

	common079721_2351(engineConfiguration, boardConfiguration);

	setFrankenso_01_LCD(boardConfiguration);
	setFrankenso0_1_joystick(engineConfiguration);

	setDensoTODO(config);

	engineConfiguration->globalFuelCorrection = 0.75;
	engineConfiguration->specs.displacement = 1.839;
//	engineConfiguration->algorithm = LM_PLAIN_MAF;
	setAlgorithm(LM_SPEED_DENSITY PASS_ENGINE_PARAMETER);
//	engineConfiguration->algorithm = LM_REAL_MAF;

	setFuelLoadBin(1.2, 4.4 PASS_ENGINE_PARAMETER);
	setFuelRpmBin(800, 7000 PASS_ENGINE_PARAMETER);

	config->veRpmBins[0] = 800;
	config->veRpmBins[1] = 1200;
	config->veRpmBins[2] = 1600;
	config->veRpmBins[3] = 2000;
	config->veRpmBins[4] = 2400;
	config->veRpmBins[5] = 2800;
	config->veRpmBins[6] = 3200;
	config->veRpmBins[7] = 3600;
	config->veRpmBins[8] = 4100;
	config->veRpmBins[9] = 4500;
	config->veRpmBins[10] = 4900;
	config->veRpmBins[11] = 5300;
	config->veRpmBins[12] = 5700;
	config->veRpmBins[13] = 6100;
	config->veRpmBins[14] = 6500;
	config->veRpmBins[15] = 7000;

	copyFuelTable(racingFestivaVeTable, config->veTable);

//	boardConfiguration->triggerInputPins[0] = GPIOC_6; // 2G YEL/BLU
//	boardConfiguration->triggerInputPins[1] = GPIOA_5; // 2E White CKP

	// in case of SOHC distributor we only have one signal
//	boardConfiguration->triggerInputPins[0] = GPIOA_5; // 2E White CKP
//	boardConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;

	// in case of DOHC distributor we have two signals
	boardConfiguration->triggerInputPins[0] = GPIOC_6;
	boardConfiguration->triggerInputPins[1] = GPIOA_5; // 2E White CKP

	// Denso 195500-2180
	engineConfiguration->injector.flow = 265;

	engineConfiguration->hasBaroSensor = false;

	engineConfiguration->hasMapSensor = true;
	boardConfiguration->isFastAdcEnabled = true;
	engineConfiguration->map.sensor.type = MT_DENSO183;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;

	setEgoSensor(ES_Innovate_MTX_L PASS_ENGINE_PARAMETER);
	engineConfiguration->afr.hwChannel = EFI_ADC_2; // Frankenso analog #5

	// set_idle_position 10
	boardConfiguration->manIdlePosition = 10;

	setWholeIatCorrTimingTable(0 PASS_ENGINE_PARAMETER);


	// set_global_trigger_offset_angle -37
	engineConfiguration->globalTriggerAngleOffset = -37;
	// set_ignition_offset 0
	engineConfiguration->ignitionOffset = 0;
	// set_injection_offset 0
	engineConfiguration->injectionOffset = 0;

	// todo: change to 15?
	// set_cranking_timing_angle 3
	engineConfiguration->crankingTimingAngle = 3;
	engineConfiguration->crankingChargeAngle = 70;
	// set_cranking_fuel 9
	engineConfiguration->cranking.baseFuel = 9;

	setTableBin2(config->ignitionLoadBins, IGN_LOAD_COUNT, 20, 105, 5);
	setWholeTimingTable(10 PASS_ENGINE_PARAMETER);
	// set_whole_fuel_map 5
	setWholeFuelMap(5 PASS_ENGINE_PARAMETER);
	setMap(config->afrTable, 13.5);

	setSingleCoilDwell(engineConfiguration);
	engineConfiguration->ignitionMode = IM_ONE_COIL;

	boardConfiguration->triggerSimulatorPinModes[0] = OM_OPENDRAIN;
	boardConfiguration->triggerSimulatorPinModes[1] = OM_OPENDRAIN;

	// individual coils
	// W6  PC9
	// W8  PC7
	// W12 PE8
	// W13 PE12

	boardConfiguration->ignitionPins[0] = GPIOC_9;
	boardConfiguration->ignitionPins[1] = GPIOC_7;
	boardConfiguration->ignitionPins[2] = GPIOE_8;
	boardConfiguration->ignitionPins[3] = GPIOE_12;
	boardConfiguration->ignitionPinMode = OM_DEFAULT;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;

	engineConfiguration->dizzySparkOutputPin = GPIOE_14; // Frankenso high side - pin 1G;

	/**
	 * Outputs
	 */
	// Frankenso low out #1: PE6
	// Frankenso low out #2: PE5 MIL
	// Frankenso low out #3:
	// Frankenso low out #4:
	// Frankenso low out #5: PE3 VICS solenoid
	// Frankenso low out #6: PE4
	// Frankenso low out #7: PE0<>PD5
	// Frankenso low out #8: PE2 INJ
	// Frankenso low out #9: PB9 IDLE
	// Frankenso low out #10: PE1<>PD3 INJ 1&3
	// Frankenso low out #11: PB8
	// Frankenso low out #12: PB7

	boardConfiguration->injectionPins[0] = GPIOD_3;
	boardConfiguration->injectionPins[1] = GPIOE_2;


	//setDefaultCrankingFuel(engineConfiguration);
	engineConfiguration->cranking.baseFuel = 5;

	// 40% idle is good default
	boardConfiguration->idle.solenoidFrequency = 300;
	boardConfiguration->idle.solenoidPin = GPIOB_9;

	boardConfiguration->malfunctionIndicatorPin = GPIOE_5;
	boardConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;

	boardConfiguration->tunerStudioSerialSpeed = 19200;

	commonFrankensoAnalogInputs(engineConfiguration);
	setCommonNTCSensor(&engineConfiguration->clt);
	engineConfiguration->clt.config.bias_resistor = 2700;
	setCommonNTCSensor(&engineConfiguration->iat);
	engineConfiguration->iat.config.bias_resistor = 2700;

	// we have a 1999 Auto Miata TB mounted on this car
	engineConfiguration->hasTpsSensor = true;
	engineConfiguration->tpsMin = 115; // convert 12to10 bit (ADC/4)
	engineConfiguration->tpsMax = 630; // convert 12to10 bit (ADC/4)

	engineConfiguration->tpsAdcChannel = EFI_ADC_3;
//	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;
	engineConfiguration->mafAdcChannel = EFI_ADC_0;
	engineConfiguration->clt.adcChannel = EFI_ADC_12;
	engineConfiguration->iat.adcChannel = EFI_ADC_11;

	// todo: 8.2 or 10k?
	engineConfiguration->vbattDividerCoeff = ((float) (10 + 33)) / 10 * 2;

	// VICS solenoid
	/**
	 * to test
	 * set_fsio_setting 0 5000
	 */
	boardConfiguration->fsio_setting[0] = 5000;
	// set_fsio_expression 1 "rpm 0 fsio_setting >"
	setFsioExt(0, GPIOE_3, "rpm 0 fsio_setting >", 150 PASS_ENGINE_PARAMETER);


	// warning light
	/**
	 * to test
	 * set_fsio_setting 1 1800
	 * set_fsio_setting 2 95
	 * set_fsio_setting 3 14
	 *
	 * set_fsio_expression 1 "rpm 0 fsio_setting > coolant 1 fsio_setting > | vbatt 2 fsio_setting < |"
	 * eval "rpm 0 fsio_setting > coolant 1 fsio_setting > | vbatt 2 fsio_setting < |"
	 */
	boardConfiguration->fsio_setting[1] = 6200; // RPM threshold
	boardConfiguration->fsio_setting[2] = 90; // CLT threshold
	boardConfiguration->fsio_setting[3] = 13.5; // voltage threshold

	setFsio(1, GPIOC_13, "rpm 1 fsio_setting > coolant 2 fsio_setting > | vbatt 3 fsio_setting < |" PASS_ENGINE_PARAMETER);


	config->ignitionRpmBins[0] = 800;
	config->ignitionRpmBins[1] = 1200;
	config->ignitionRpmBins[2] = 1600;
	config->ignitionRpmBins[3] = 2000;
	config->ignitionRpmBins[4] = 2400;
	config->ignitionRpmBins[5] = 2800;
	config->ignitionRpmBins[6] = 3200;
	config->ignitionRpmBins[7] = 3600;
	config->ignitionRpmBins[8] = 4100;
	config->ignitionRpmBins[9] = 4500;
	config->ignitionRpmBins[10] = 4900;
	config->ignitionRpmBins[11] = 5300;
	config->ignitionRpmBins[12] = 5700;
	config->ignitionRpmBins[13] = 6100;
	config->ignitionRpmBins[14] = 6500;
	config->ignitionRpmBins[15] = 7000;
	copyTimingTable(racingFestivaIgnitionTable, config->ignitionTable);

	//	boardConfiguration->useWarmupPidAfr = true;
		engineConfiguration->warmupAfrPid.pFactor = -0.2;
		engineConfiguration->warmupAfrPid.iFactor = -0.0005;
	//	engineConfiguration->warmupAfrPid.dFactor = -0.02;
		engineConfiguration->debugMode = WARMUP_ENRICH;


	engineConfiguration->tpsAccelEnrichmentThreshold = 40;
	engineConfiguration->tpsAccelEnrichmentMultiplier = 0.0;

	engineConfiguration->engineLoadAccelEnrichmentThreshold = 5.0;
	engineConfiguration->engineLoadAccelEnrichmentMultiplier = 1;

	boardConfiguration->isSdCardEnabled = true;

	// end of Ford Escort GT config
}

