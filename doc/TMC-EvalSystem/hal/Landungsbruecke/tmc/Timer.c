#include "hal/HAL.h"
#include "hal/Timer.h"

#define MAX_ARR_HALF  TIMER_MAX >> 1

static void init(void);
static void deInit(void);
static void setDuty(timer_channel, uint16_t);
static uint16_t getDuty(timer_channel);

TimerTypeDef Timer =
{
	.init     = init,
	.deInit   = deInit,
	.setDuty  = setDuty,
	.getDuty  = getDuty
};

static void init(void)
{
	// enable clock for FTM0
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;

	// enable clock for port D
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;

	// disable write protection
	FTM0_MODE |= FTM_MODE_WPDIS_MASK;

	// FAULTM = 1 - Fault control is enabled for all channels,
	// FTMEN  = 1 - all registers are available for use with no restrictions.
	FTM0_MODE |= FTM_MODE_FAULTM_MASK | FTM_MODE_FTMEN_MASK;

	// setting for Center Aligned PWM in Combine Mode
	FTM0_MOD = TIMER_MAX;  // set PWM frequency
	FTM0_CNTIN = 0;        // CTNMAX = 1 - PWM update at counter in max. value
	FTM0_SYNC |= FTM_SYNC_CNTMAX_MASK;

	// SWSYNC = 1 - set PWM value update. This bit is cleared automatically.
	FTM0_SYNC |= FTM_SYNC_SWSYNC_MASK;

	// disable all channels outputs using the OUTPUT MASK feature
	FTM0_OUTMASK = FTM_OUTMASK_CH0OM_MASK | FTM_OUTMASK_CH1OM_MASK
	             | FTM_OUTMASK_CH4OM_MASK | FTM_OUTMASK_CH5OM_MASK
	             | FTM_OUTMASK_CH6OM_MASK | FTM_OUTMASK_CH7OM_MASK;

	/* COMBINE = 1 - combine mode set, COMP = 1 - complementary PWM set,
	DTEN = 1 - deadtime enabled, SYNCEN = 1 - PWM update synchronization enabled,
	FAULTEN = 1 - fault control enabled */
	FTM0_COMBINE = FTM_COMBINE_SYNCEN0_MASK | FTM_COMBINE_DTEN0_MASK
	             | FTM_COMBINE_COMP0_MASK   | FTM_COMBINE_COMBINE0_MASK
	             | FTM_COMBINE_SYNCEN2_MASK | FTM_COMBINE_DTEN2_MASK
	             | FTM_COMBINE_COMP2_MASK   | FTM_COMBINE_COMBINE2_MASK
	             | FTM_COMBINE_SYNCEN3_MASK | FTM_COMBINE_DTEN3_MASK
	             | FTM_COMBINE_COMP3_MASK   | FTM_COMBINE_COMBINE3_MASK;

	// initialize setting of value registers to  duty cycle
	FTM0_C0V = 0;
	FTM0_C1V = MAX_ARR_HALF;
	FTM0_C4V = 0;
	FTM0_C5V = MAX_ARR_HALF;
	FTM0_C6V = 0;
	FTM0_C7V = MAX_ARR_HALF;

	// set channel mode to generate positive PWM
	FTM0_C0SC |= FTM_CnSC_ELSB_MASK;
	FTM0_C1SC |= FTM_CnSC_ELSB_MASK;
	FTM0_C4SC |= FTM_CnSC_ELSB_MASK;
	FTM0_C5SC |= FTM_CnSC_ELSB_MASK;
	FTM0_C6SC |= FTM_CnSC_ELSB_MASK;
	FTM0_C7SC |= FTM_CnSC_ELSB_MASK;

	// enable loading of the MOD, CNTIN, and CV registers with the values of their write buffers
	FTM0_PWMLOAD = FTM_PWMLOAD_LDOK_MASK;

	// enable the generation of the trigger when the FTM counter is equal to the CNTIN register
	FTM0_EXTTRIG |= FTM_EXTTRIG_INITTRIGEN_MASK;
	FTM0_MODE |= FTM_MODE_INIT_MASK;

	// set system clock as source for FTM0 (CLKS[1:0] = 01)
	FTM0_SC |= FTM_SC_CLKS(1); // Periodendauer 166,66us

	// initialize pwm pin for FTM0




	// ATTENTION The Pins had to configure as AF4 near the initialisation !!!!!!




	// enable PWM outputs of FTM0
	FTM0_OUTMASK = 0;
}

static void deInit(void)
{
	SIM_SCGC6 &= ~SIM_SCGC6_FTM0_MASK;
}

static void setDuty(timer_channel channel, uint16_t duty)
{
	switch(channel) {
	case TIMER_CHANNEL_2:
		FTM0_C1V = (duty < TIMER_MAX) ? duty : TIMER_MAX;
		break;
	case TIMER_CHANNEL_3:
		FTM0_C5V = (duty < TIMER_MAX) ? duty : TIMER_MAX;
		break;
	case TIMER_CHANNEL_1:
	default:
		FTM0_C7V = (duty < TIMER_MAX) ? duty : TIMER_MAX;
		break;
	}
	FTM0_PWMLOAD = FTM_PWMLOAD_LDOK_MASK;
}

static uint16_t getDuty(timer_channel channel)
{
	uint16_t duty = 0;
	switch(channel) {
	case TIMER_CHANNEL_2:
		duty = (FTM0_C1V - FTM0_C0V);
		break;
	case TIMER_CHANNEL_3:
		duty = (FTM0_C5V - FTM0_C4V);
		break;
	case TIMER_CHANNEL_1:
	default:
		duty = (FTM0_C7V - FTM0_C6V);
		break;
	}
	return duty;
}
