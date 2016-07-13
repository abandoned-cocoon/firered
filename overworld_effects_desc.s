.set OEC_LOAD_GFX, 0
.set OEC_LOAD_PAL1, 1
.set OEC_LOAD_PAL2, 2
.set OEC_CALL, 3
.set OEC_END, 4
.set OEC_GFX_PAL1_AND_CALL, 5
.set OEC_GFX_AND_CALL, 6
.set OEC_PAL1_AND_CALL, 7

/* 081D96AC */
oe_081D97C4:
	.byte OEC_CALL
	.long sub_08082544
	.byte OEC_END

/* 081D97CA */
oe_cut:
	.byte OEC_CALL
	.long oei_cut
	.byte OEC_END

/* 081D97D0 */
oe_cut2:
	.byte OEC_CALL
	.long oei_cut2
	.byte OEC_END

/* 081D97D6 */
oe_081D97D6:
	.byte OEC_CALL
	.long sub_080DB1DC
	.byte OEC_END

/* 081D96AC */
oe_table:
	.long oe_081D97C4
	.long oe_cut
	.long oe_cut2
	.long oe_081D97D6
	/* ... */
