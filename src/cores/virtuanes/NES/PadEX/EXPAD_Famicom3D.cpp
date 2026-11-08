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
