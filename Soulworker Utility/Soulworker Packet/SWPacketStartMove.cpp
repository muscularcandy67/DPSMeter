#include "pch.h"
#include ".\Soulworker Packet\SWPacket.h"
#include ".\Damage Meter\Damage Meter.h"
#include ".\Soulworker Packet\SWPacketStartMove.h"

SWPacketStartMove::SWPacketStartMove(SWHEADER* swheader, BYTE* data) {
	_swheader = swheader;
	_data = data;
}

VOID SWPacketStartMove::Do() {
	return;
}

VOID SWPacketStartMove::Log() {
	return;
}

VOID SWPacketStartMove::Debug() {

	//USHORT op = _byteswap_ushort(_swheader->_op);
	//_SWPACKETSTARTMOVE* move = (_SWPACKETSTARTMOVE*)(_data + sizeof(SWHEADER));

	//if (_swheader->_op != 0x3403) {
	//	return;
	//}

	//printf("OP : %04x\tsize : %04x\n", op, _swheader->_size);

	//
	//for (int i = 0; i < _swheader->_size; i++)
	//	printf("%02x ", _data[i]);
	//printf("\n");

	//LogInstance.MyLog(_T("Unkown Packet : %04x\n"), op);
	//for (int i = sizeof(SWHEADER); i < _swheader->_size; i++)
	//	LogInstance.MyLog(_T("%02x "), _data[i]);
	//LogInstance.MyLog(_T("\n"));

	//LogInstance.MyLog(_T("player ID : %08x / map ID : %u / X : %f, Y : %f, Z : %f\n"), move->_playerId, move->_mapId, move->_locationX, move->_locationY, move->_locationZ);
	//LogInstance.MyLog(_T("direction %f / endLocationX %f / endLocationX %f\n"), move->_direction, move->_endLocationX, move->_endLocationY);
	//LogInstance.MyLog(_T("speed : %f\n "), move->_speed);

	//LogInstance.MyLog(_T("Unknown01 : "));
	//for (int i = 0; i < sizeof(move->_unknown01); i++) {
	//	LogInstance.MyLog(_T("%02x "), move->_unknown01[i]);
	//}
	//LogInstance.MyLog(_T(" / "));

	//LogInstance.MyLog(_T("Unknown02 : "));
	//for (int i = 0; i < sizeof(move->_unknown02); i++) {
	//	LogInstance.MyLog(_T("%02x "), move->_unknown02[i]);
	//}
	//LogInstance.MyLog(_T(" / "));

	//LogInstance.MyLog(_T("_screen : "));
	//for (int i = 0; i < sizeof(move->_screen); i++) {
	//	LogInstance.MyLog(_T("%02x "), move->_screen[i]);
	//}
	//LogInstance.MyLog(_T(" / \n"));
	//
	//LogInstance.MyLog(_T("unknown 04 : %02x"), move->_unknown04);

	//LogInstance.MyLog(_T(" / "));

	//LogInstance.MyLog(_T("Unknown05 : "));
	//for (int i = 0; i < sizeof(move->_unknown05); i++) {
	//	LogInstance.MyLog(_T("%02x "), move->_unknown05[i]);
	//}
	//LogInstance.MyLog(_T(" / \n"));

	return;
}