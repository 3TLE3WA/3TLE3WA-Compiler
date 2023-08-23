#include "3tle3wa/backend/rl/InternalTranslation.hh"
#include "3tle3wa/ir/instHeader.hh"

// control flow inst
void ReturnInst::TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) { itx.Translate(this, ctx); }

void JumpInst::TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) { itx.Translate(this, ctx); }

void BranchInst::TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) { itx.Translate(this, ctx); }

// compare inst
void ICmpInst::TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) { itx.Translate(this, ctx); }

void FCmpInst::TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) { itx.Translate(this, ctx); }

// binary inst
void IBinaryInst::TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) { itx.Translate(this, ctx); }

void FBinaryInst::TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) { itx.Translate(this, ctx); }

void FNegInst::TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) { itx.Translate(this, ctx); }

void FAbsInst::TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) { itx.Translate(this, ctx); }

// memory inst
void AllocaInst::TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) { itx.Translate(this, ctx); }

void StoreInst::TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) { itx.Translate(this, ctx); }

void LoadInst::TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) { itx.Translate(this, ctx); }

void GetElementPtrInst::TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) { itx.Translate(this, ctx); }

// type convert
void SitoFpInst::TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) { itx.Translate(this, ctx); }

void FptoSiInst::TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) { itx.Translate(this, ctx); }

void ZextInst::TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) { itx.Translate(this, ctx); }

// other inst
void CallInst::TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) { itx.Translate(this, ctx); }

void BitCastInst::TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) { itx.Translate(this, ctx); }

void PhiInst::TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) { itx.Translate(this, ctx); }