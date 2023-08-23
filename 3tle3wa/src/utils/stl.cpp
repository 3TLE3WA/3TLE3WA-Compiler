/**
 * @name Small Thread Library
 * @brief a thread lib for automatic parallelization
 * @author 3tle3wa
 *
 * @attention please follow the license
 */

/*
# from syscall.h

.equ	__NR_exit, 93
.equ	__NR_waitid, 95
.equ	__NR_clone, 220
.equ	__NR_sched_setaffinity, 122

# from signal.h

.equ	SIGCHLD, 17

# from sched.h

.equ	CLONE_VM, 0x00000100

# from waitflags.h

.equ	WEXITED, 4

.equ	P_ALL, 0

# from stdlib

.equ	EXIT_FAILURE, 1
.equ	EXIT_SUCCESS, 0

        .data
        .align	4
        .type	.LC.lock, @object
        .size	.LC.lock, 4
.LC.lock:
        .zero	4

        .text
        .align	1
        .type	__crvc_fork, @function
        .global	__crvc_fork
__crvc_fork:
        li	a7, __NR_clone
        mv	a5, zero
        mv	a4, zero
        mv	a3, zero
        mv	a2, zero
        mv	a1, sp
        li	a0, SIGCHLD | CLONE_VM
        ecall
        ret
        .size	__crvc_fork, .-__crvc_fork

        .text
        .align	1
        .type	__crvc_waitid, @function
        .global	__crvc_waitid
__crvc_waitid:
        li	a7, __NR_waitid
        mv	a4, zero
        li	a3, WEXITED
        mv	a2, zero
        mv	a1, zero
        li	a0, P_ALL
        ecall
        ret
        .size	__crvc_waitid, .-__crvc_waitid

        .text
        .align	1
        .type	__crvc_exit, @function
        .global	__crvc_exit
__crvc_exit:
        li	a7, __NR_exit
        li	a0, EXIT_SUCCESS
        ecall
        .size	__crvc_exit, .-__crvc_exit

        .text
        .align	1
        .type	__crvc_spinlock_lock, @function
        .global	__crvc_spinlock_lock
__crvc_spinlock_lock:
        lla	t0, .LC.lock
        li	t1, 1
.trylock:
        lw	t2, 0(t0)
        bnez	t2, .trylock
        amoswap.w.aq	t2, t1, 0(t0)
        bnez	t2, .trylock
        ret
        .size	__crvc_spinlock_lock, .-__crvc_spinlock_lock

        .text
        .align	1
        .type	__crvc_spinlock_unlock, @function
        .global	__crvc_spinlock_unlock
__crvc_spinlock_unlock:
        lla	t0, .LC.lock
        amoswap.w.rl	zero, zero, 0(t0)
        ret
        .size	__crvc_spinlock_unlock, .-__crvc_spinlock_unlock
*/

const char *libstl = R"(# experimental
	.data
	.align	4
	.type	.LC.lock, @object
	.size	.LC.lock, 4
.LC.lock:
	.zero	4
	.text
	.align	1
	.type	__crvc_fork, @function
	.global	__crvc_fork
__crvc_fork:
	li	a7, 220
	mv	a5, zero
	mv	a4, zero
	mv	a3, zero
	mv	a2, zero
	mv	a1, sp
	li	a0, 273
	ecall
	ret
	.size	__crvc_fork, .-__crvc_fork
	.text
	.align	1
	.type	__crvc_waitid, @function
	.global	__crvc_waitid
__crvc_waitid:
	li	a7, 95
	mv	a4, zero
	li	a3, 4
	mv	a2, zero
	mv	a1, zero
	li	a0, 0
	ecall
	ret
	.size	__crvc_waitid, .-__crvc_waitid
	.text
	.align	1
	.type	__crvc_exit, @function
	.global	__crvc_exit
__crvc_exit:
	li	a7, 93
	li	a0, 0
	ecall
	.size	__crvc_exit, .-__crvc_exit
	.text
	.align	1
	.type	__crvc_spinlock_lock, @function
	.global	__crvc_spinlock_lock
__crvc_spinlock_lock:
	lla	t0, .LC.lock
	li	t1, 1
.trylock:
	lw	t2, 0(t0)
	bnez	t2, .trylock
	amoswap.w.aq	t2, t1, 0(t0)
	bnez	t2, .trylock
	ret
	.size	__crvc_spinlock_lock, .-__crvc_spinlock_lock
	.text
	.align	1
	.type	__crvc_spinlock_unlock, @function
	.global	__crvc_spinlock_unlock
__crvc_spinlock_unlock:
	lla	t0, .LC.lock
	amoswap.w.rl	zero, zero, 0(t0)
	ret
	.size	__crvc_spinlock_unlock, .-__crvc_spinlock_unlock
)";