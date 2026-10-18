//////////////////////////////////////////////////////////////////////////
// Famicom 3D System                                                   //
//////////////////////////////////////////////////////////////////////////
void EXPAD_Famicom3D::Reset()
{
	eyeRight = FALSE;
	framesSinceEyeChange = FAMICOM3D_ACTIVITY_FRAMES + 1;
}

void EXPAD_Famicom3D::Write4016( BYTE data )
{
	BOOL newEyeRight = (data & FAMICOM3D_EYE_MASK) ? TRUE : FALSE;

	if( newEyeRight != eyeRight ) {
		eyeRight = newEyeRight;
		framesSinceEyeChange = 0;
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
