#pragma once
#define FUNC(address, return_t, callconv, name, ...) return_t (callconv* name)(__VA_ARGS__) = reinterpret_cast<decltype(name)>(address)
#define INLINE_FUNC(address, return_t, callconv, name, ...) inline return_t (callconv* name)(__VA_ARGS__) = reinterpret_cast<decltype(name)>(address)
#define SAFE_CALL(obj, method, ...) if(obj) obj->method(__VA_ARGS__)

#define SAVE_REGS_EAX __asm\
{\
	__asm push ebx\
	__asm push ecx\
	__asm push edx\
	__asm push edi\
	__asm push esi\
}\

#define RESTORE_REGS_EAX __asm\
{\
	__asm pop esi\
	__asm pop edi\
	__asm pop edx\
	__asm pop ecx\
	__asm pop ebx\
}\

#define SAVE_REGS_ESI __asm\
{\
	__asm push eax\
	__asm push ebx\
	__asm push ecx\
	__asm push edx\
	__asm push edi\
}\

#define RESTORE_REGS_ESI __asm\
{\
	__asm pop edi\
	__asm pop edx\
	__asm pop ecx\
	__asm pop ebx\
	__asm pop eax\
}\

;