/**
 * @name Fast Small Thread Library
 * @brief a risc thread lib for automatic parallelization
 * @author 3tle3wa
 *
 * @attention please follow the license
 */

const char *libfstl = R"(
	.equ	SYS_exit, 93
	.equ	SYS_sched_yield, 158
	.equ	SYS_clone, 220
	.equ	CLONE_VM, 0x00000100
	.equ	EXIT_SUCCESS, 0
	.data
	.align	4
	.type	.LC.tcreated, @object
	.size	.LC.tcreated, 4
.LC.tcreated:
	.zero	4
	.data
	.align	4
	.type	.LC.texited, @object
	.size	.LC.texited, 4
.LC.texited:
	.zero	4
	.text
	.align	1
	.globl	__crvc_fork
	.type	__crvc_fork, @function
	.global	__crvc_fork
__crvc_fork:
	lw	a0, .LC.tcreated
	addi	a0, a0, 1
	sw	a0, .LC.tcreated, a1
	li	a7, SYS_clone
	mv	a5, zero
	mv	a4, zero
	mv	a3, zero
	mv	a2, zero
	mv	a1, sp
	li	a0, CLONE_VM
	ecall
	ret
	.size	__crvc_fork, .-__crvc_fork
	.text
	.align	1
	.globl	__crvc_exit
	.type	__crvc_exit, @function
	.global	__crvc_exit
__crvc_exit:
	lla	a0, .LC.texited
	li	a1, 1
	amoadd.w.aqrl	zero, a1, 0(a0)
	li	a7, SYS_exit
	li	a0, EXIT_SUCCESS
	ecall
	.size	__crvc_exit, .-__crvc_exit
	.text
	.align	1
	.globl	__crvc_pass
	.type	__crvc_pass, @function
	.global	__crvc_pass
__crvc_pass:
	ld	a1, .LC.tcreated
	lla	a2, .LC.texited
.trypass:
	li	a7, SYS_sched_yield
	ecall
	amoor.w.aqrl	a3, zero, 0(a2)
	bne	a3, a1, .trypass
	ret
	.size	__crvc_pass, .-__crvc_pass
	.data
	.align	4
	.type	.LC.lock, @object
	.size	.LC.lock, 4
.LC.lock:
	.zero	4
	.text
	.align	1
	.type	__crvc_spinlock_lock, @function
	.global	__crvc_spinlock_lock
__crvc_spinlock_lock:
	lla	a1, .LC.lock
	li	a2, 1
	lw	a3, 0(a1)
	bnez	a3, .trylock
	amoswap.w.aq	a3, a2, 0(a1)
	bnez	a3, .trylock
	ret
.trylock:
	li	a7, SYS_sched_yield
	ecall
	lw	a3, 0(a1)
	bnez	a3, .trylock
	amoswap.w.aq	a3, a2, 0(a1)
	bnez	a3, .trylock
	ret
	.size	__crvc_spinlock_lock, .-__crvc_spinlock_lock
	.text
	.align	1
	.type	__crvc_spinlock_unlock, @function
	.global	__crvc_spinlock_unlock
__crvc_spinlock_unlock:
	lla	a0, .LC.lock
	amoswap.w.rl	zero, zero, 0(a0)
	ret
	.size	__crvc_spinlock_unlock, .-__crvc_spinlock_unlock
)";