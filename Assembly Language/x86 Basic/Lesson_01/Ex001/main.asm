.386

.model flat, stdcall

.data

.code

mainCRTStartup PROC
	mov ecx, 0
;	xor ecx, ecx

my_label:	
	inc ecx
	cmp ecx, 3
	jl my_label

	mov eax, 100

mainCRTStartup ENDP
END