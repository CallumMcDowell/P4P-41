#include <stdint.h>

/* Part of above either or */
#define CUSTOM_INSTRUCT

// Optional test benches
#define CUSTOM_INSTRUCT_VMUL
#define CUSTOM_INSTRUCT_VACC
#define CUSTOM_INSTRUCT_VMAXE_VMINE_VMAX_X
#define CUSTOM_INSTRUCT_VSRLI

#ifdef CUSTOM_INSTRUCT
	// P4P Custom Instructions
	// ---------------------------------------
	// Must Have
	// ---------------------------------------
	#ifdef CUSTOM_INSTRUCT_VMUL
	uint32_t _vmul(uint32_t rd, uint32_t r1, uint32_t r2);
	#endif

	#ifdef CUSTOM_INSTRUCT_VACC
	uint32_t _vacc(uint32_t rd, uint32_t r1);
	#endif

	#ifdef CUSTOM_INSTRUCT_VMAXE_VMINE_VMAX_X
	uint32_t _vmaxe(uint32_t rd, uint32_t r1);
	uint32_t _vmine(uint32_t rd, uint32_t r1);
	uint32_t _vmax_x(uint32_t rd, uint32_t r1, uint32_t r2);
	#endif

	#ifdef CUSTOM_INSTRUCT_VSRLI
	uint32_t _vsrli(uint32_t rd, uint32_t r1);
	#endif

#endif