#pragma once

#include <conio.h>
#include <Windows.h>

namespace yuh
{
	class console
	{
		HANDLE hStdout_;

	public:
		enum class Key
		{
			NUL			= 0x00, //     �inull����)
			SOH			= 0x01, // C-a �i�w�b�_�J�n�j
			STX			= 0x02, // C-b �i�e�L�X�g�J�n�j
			ETX			= 0x03, //     �i�e�L�X�g�I���j
			EOT			= 0x04, // C-d �i�]���I���j
			ENQ			= 0x05, // C-e �i�Ɖ�j
			ACK			= 0x06, // C-f �i��M�n�j�j
			BEL			= 0x07, // C-g �i�x���j
			BS			= 0x08, // C-h �i��ށj
			HT			= 0x09, // C-i �i�����^�u�j
			LF			= 0x0a, // C-j �i���s�j
			VT			= 0x0b, // C-k �i�����^�u�j
			FF			= 0x0c, // C-l �i���Łj
			CR			= 0x0d, // C-m �i���A�j
			SO			= 0x0e, // C-n �i�V�t�g�A�E�g�j
			SI			= 0x0f, // C-o �i�V�t�g�C���j
			DLE			= 0x10, // C-p �i�f�[�^�����N�G�X�P�[�v�j
			DC1			= 0x11, // C-q �i���u����P�j
			DC2			= 0x12, // C-r �i���u����Q�j
			DC3			= 0x13, //    �i���u����R�j
			DC4			= 0x14, // C-t �i���u����S�j
			NAK			= 0x15, // C-u �i��M���s�j
			SYN			= 0x16, // C-v �i�����j
			ETB			= 0x17, // C-w �i�]���u���b�N�I���j
			CAN			= 0x18, // C-x �i�Ƃ肯���j
			EM			= 0x19, // C-y �i���f�B�A�I���j
			SUB			= 0x1a, // C-z �i�u���j
			ESC			= 0x1b, // C-[ �i�G�X�P�[�v�j
			FS			= 0x1c, // C- �i�t�H�[����؂�j
			GS			= 0x1d, // C- �i�O���[�v��؂�j
			RS			= 0x1e, // C- �i���R�[�h��؂�j
			US			= 0x1f, // C- �i���j�b�g��؂�j

			F1			= 0x3B00,
			F2			= 0x3C00,
			F3			= 0x3D00,
			F4			= 0x3E00,
			F5			= 0x3F00,
			F6			= 0x4000,
			F7			= 0x4100,
			F8			= 0x4200,
			F9			= 0x4300,
			F10			= 0x4400,
			HOME		= 0x47E0,
			KEYUP		= 0x48E0,
			PAGEUP		= 0x49E0,
			KEYLEFT		= 0x4BE0,
			KEYRIGHT	= 0x4DE0,
			END			= 0x4FE0,
			KEYDOWN		= 0x50E0,
			PAGEDOWN	= 0x51E0,
			DEL			= 0x53E0,
			INSERT		= 0x52E0,
			F11			= 0x85E0,
			F12			= 0x86E0,
			
		};

		console();

		/**
		 * �R���\�[�����̓��C�����[�v
		 * @param swt �L�[����->
		 */
		template<
			typename F1,
			typename F2
		>
		inline void loop(F1 swt, F2 turn)
		{
			while (turn()) {
				if (_kbhit())
				{
					auto key = _getch();
					if( key == 0x00 || key == 0xe0 )
						key |= _getch() << 8;
					if(!swt(key)){ break; }
				}
			}
		}

		inline void setPos(int x, int y)
		{
			COORD coord = { x, y };

			::SetConsoleCursorPosition(
				hStdout_,
				coord
				);
		}
	};
}