//////////////////////////////////////////////////////////////////////////
// Famicom 3D System                                                   //
//////////////////////////////////////////////////////////////////////////
void EXPAD_Famicom3D::Reset()
{
	eyeRight = FALSE;
	framesSinceEyeChange = FAMICOM3D_ACTIVITY_FRAMES + 1;
	lastWriteLowBits = 0;
#if FAMICOM3D_DEBUG
	for( INT bit = 0; bit < 3; bit++ )
		debugBitTransitions[bit] = 0;
	debugWriteCount = 0;
	debugLastChangeScanline = -1;
	debugMinChangeScanline = -1;
	debugMaxChangeScanline = -1;
	for( INT i = 0; i < 16; i++ )
		debugEyeHistory[i] = 0;
	debugEyeHistoryNext = 0;
	debugEyeHistoryCount = 0;
	debugHasPreviousWrite = FALSE;
#endif
}

void EXPAD_Famicom3D::Write4016( BYTE data )
{
	BYTE lowBits = data & 0x07;
	BOOL newEyeRight = (data & FAMICOM3D_EYE_MASK) ? TRUE : FALSE;

#if FAMICOM3D_DEBUG
	if( debugHasPreviousWrite ) {
		for( INT bit = 0; bit < 3; bit++ ) {
			if( ((lowBits ^ lastWriteLowBits) & (1 << bit)) != 0 )
				debugBitTransitions[bit]++;
		}
	}
	debugWriteCount++;
	debugHasPreviousWrite = TRUE;
#endif
	lastWriteLowBits = lowBits;

	if( newEyeRight != eyeRight ) {
		eyeRight = newEyeRight;
		framesSinceEyeChange = 0;
#if FAMICOM3D_DEBUG
		debugLastChangeScanline = nes->GetScanline();
		if( debugMinChangeScanline < 0 || debugLastChangeScanline < debugMinChangeScanline )
			debugMinChangeScanline = debugLastChangeScanline;
		if( debugMaxChangeScanline < 0 || debugLastChangeScanline > debugMaxChangeScanline )
			debugMaxChangeScanline = debugLastChangeScanline;
#endif
	}
}

BOOL EXPAD_Famicom3D::IsActive() const
{
	return framesSinceEyeChange <= FAMICOM3D_ACTIVITY_FRAMES;
}

void EXPAD_Famicom3D::AdvanceFrame()
{
	if( framesSinceEyeChange <= FAMICOM3D_ACTIVITY_FRAMES ) {
		framesSinceEyeChange++;
	}
}

void EXPAD_Famicom3D::SetSyncData( INT type, LONG data )
{
	if( type == 0 ) {
		eyeRight = (data & 0x01) ? TRUE : FALSE;
		framesSinceEyeChange = (BYTE)((data >> 1) & 0x0F);
		if( framesSinceEyeChange > FAMICOM3D_ACTIVITY_FRAMES + 1 ) {
			framesSinceEyeChange = FAMICOM3D_ACTIVITY_FRAMES + 1;
		}
	}
}

LONG EXPAD_Famicom3D::GetSyncData( INT type )
{
	if( type == 0 ) {
		return (eyeRight ? 0x01 : 0x00) | ((LONG)framesSinceEyeChange << 1);
	}
	return 0;
}

#if FAMICOM3D_DEBUG
void EXPAD_Famicom3D::RecordFrameStart()
{
	debugEyeHistory[debugEyeHistoryNext] = eyeRight ? 1 : 0;
	debugEyeHistoryNext = (debugEyeHistoryNext + 1) & 0x0F;
	if( debugEyeHistoryCount < 16 )
		debugEyeHistoryCount++;
}

void EXPAD_Famicom3D::GetDebugInfo( INT transitions[3], INT& writes,
	INT& lastChangeScanline, INT& minChangeScanline, INT& maxChangeScanline,
	char eyeHistory[17], INT& active, INT& framesSinceChange )
{
	for( INT bit = 0; bit < 3; bit++ )
		transitions[bit] = debugBitTransitions[bit];
	writes = debugWriteCount;
	lastChangeScanline = debugLastChangeScanline;
	minChangeScanline = debugMinChangeScanline;
	maxChangeScanline = debugMaxChangeScanline;
	active = IsActive() ? 1 : 0;
	framesSinceChange = framesSinceEyeChange;

	INT first = debugEyeHistoryCount == 16 ? debugEyeHistoryNext : 0;
	for( INT i = 0; i < 16; i++ )
		eyeHistory[i] = debugEyeHistory[(first + i) & 0x0F] ? '1' : '0';
	eyeHistory[16] = '\0';
}

void EXPAD_Famicom3D::ResetDebugWindow()
{
	for( INT bit = 0; bit < 3; bit++ )
		debugBitTransitions[bit] = 0;
	debugWriteCount = 0;
	debugMinChangeScanline = -1;
	debugMaxChangeScanline = -1;
}
#endif