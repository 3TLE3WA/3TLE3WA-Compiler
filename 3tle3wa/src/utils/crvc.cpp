/**
 * @name C2RV lib for high performance
 * @brief a risc lib
 * @author 3tle3wa
 *
 * @attention please follow the license
 */

const char *libcrvc = R"(
memset:
.L.memset.entry64:
	li	a3, 64
.L.memset.try64:
	blt	a2, a3, .L.memset.entry32
	sd	a1, 0(a0)
	sd	a1, 8(a0)
	sd	a1, 16(a0)
	sd	a1, 24(a0)
	sd	a1, 32(a0)
	sd	a1, 40(a0)
	sd	a1, 48(a0)
	sd	a1, 56(a0)
	addi	a2, a2, -64
	addi	a0, a0, 64
	bnez	a2, .L.memset.try64
.L.memset.entry32:
	li	a3, 32
.L.memset.try32:
	blt	a2, a3, .L.memset.entry16
	sd	a1, 0(a0)
	sd	a1, 8(a0)
	sd	a1, 16(a0)
	sd	a1, 24(a0)
	addi	a2, a2, -32
	addi	a0, a0, 32
	bnez	a2, .L.memset.try32
.L.memset.entry16:
	li	a3, 16
.L.memset.try16:
	blt	a2, a3, .L.memset.exit
	sd	a1, 0(a0)
	sd	a1, 8(a0)
	addi	a2, a2, -16
	addi	a0, a0, 16
	bnez	a2, .L.memset.try16
.L.memset.exit:
	ret
)";