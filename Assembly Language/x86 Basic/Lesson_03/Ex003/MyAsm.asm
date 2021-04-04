.686
.XMM

.model flat, C

option casemap :none

.code

asm_add_cpp PROC arg_dst:DWORD, arg_a:DWORD, arg_b:DWORD, arg_size:DWORD
	;pusha
	push ecx
	push esi
	push ebx
	push edi

	mov ecx, arg_size
	mov esi, arg_a
	mov ebx, arg_b
	mov edi, arg_dst

my_loop:
	movaps xmm0, DWORD PTR[esi]
	addps  xmm0, DWORD PTR[ebx]
	movaps DWORD PTR[edi], xmm0

	add esi, 16;
	add ebx, 16;
	add edi, 16;

	dec ecx
	jnz my_loop

	;popa
	pop edi
	pop ebx
	pop esi
	pop ecx
	ret
asm_add_cpp ENDP

END