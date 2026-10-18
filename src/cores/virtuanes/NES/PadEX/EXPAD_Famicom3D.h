//////////////////////////////////////////////////////////////////////////
// Famicom 3D System                                                   //
//////////////////////////////////////////////////////////////////////////
#ifndef __EXPAD_FAMICOM3D_INCLUDED__
#define __EXPAD_FAMICOM3D_INCLUDED__

class EXPAD_Famicom3D : public EXPAD
{
public:
	enum {
		FAMICOM3D_EYE_MASK = 0x02,
		FAMICOM3D_ACTIVITY_FRAMES = 3
	};

	EXPAD_Famicom3D( NES* parent ) : EXPAD( parent ) { Reset(); }

	void Reset();
	void Write4016( BYTE data );

	BOOL IsActive() const;
	BOOL IsRightEye() const { return eyeRight; }
	void AdvanceFrame();
	void SetSyncData( INT type, LONG data );
	LONG GetSyncData( INT type );

protected:
	BOOL eyeRight;
	BYTE framesSinceEyeChange;
};

#endif // !__EXPAD_FAMICOM3D_INCLUDED__