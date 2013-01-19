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
			NUL			= 0x00, //     （null文字)
			SOH			= 0x01, // C-a （ヘッダ開始）
			STX			= 0x02, // C-b （テキスト開始）
			ETX			= 0x03, //     （テキスト終了）
			EOT			= 0x04, // C-d （転送終了）
			ENQ			= 0x05, // C-e （照会）
			ACK			= 0x06, // C-f （受信ＯＫ）
			BEL			= 0x07, // C-g （警告）
			BS			= 0x08, // C-h （後退）
			HT			= 0x09, // C-i （水平タブ）
			LF			= 0x0a, // C-j （改行）
			VT			= 0x0b, // C-k （垂直タブ）
			FF			= 0x0c, // C-l （改頁）
			CR			= 0x0d, // C-m （復帰）
			SO			= 0x0e, // C-n （シフトアウト）
			SI			= 0x0f, // C-o （シフトイン）
			DLE			= 0x10, // C-p （データリンクエスケープ）
			DC1			= 0x11, // C-q （装置制御１）
			DC2			= 0x12, // C-r （装置制御２）
			DC3			= 0x13, //    （装置制御３）
			DC4			= 0x14, // C-t （装置制御４）
			NAK			= 0x15, // C-u （受信失敗）
			SYN			= 0x16, // C-v （同期）
			ETB			= 0x17, // C-w （転送ブロック終了）
			CAN			= 0x18, // C-x （とりけし）
			EM			= 0x19, // C-y （メディア終了）
			SUB			= 0x1a, // C-z （置換）
			ESC			= 0x1b, // C-[ （エスケープ）
			FS			= 0x1c, // C- （フォーム区切り）
			GS			= 0x1d, // C- （グループ区切り）
			RS			= 0x1e, // C- （レコード区切り）
			US			= 0x1f, // C- （ユニット区切り）

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
		 * コンソール入力メインループ
		 * @param swt キー入力->
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