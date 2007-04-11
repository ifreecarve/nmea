#include "nmea.h"

VALUE mGPS, mNMEA, cLatitude, cLongitude, cTime, eParseError, cSatelliteInfo;
ID id_GPS, id_Latitude, id_Longitude, id_new, id_rmc;
ID id_gsv, id_gsa, id_gga, id_psrftxt, id_vtg, id_gll;
VALUE id_start, id_continue, id_finish;
VALUE id_manual, id_automatic, id_no_fix, id_2d, id_3d;
VALUE id_autonomus, id_differential, id_estimated, id_invalid;

int load_constants() {
	if(!rb_const_defined(rb_cObject, id_GPS)) return 0;
	mGPS = rb_const_get(rb_cObject, rb_intern("GPS"));
	if(!rb_const_defined(mGPS, id_Latitude)) return 0;
	cLatitude = rb_const_get(mGPS, rb_intern("Latitude"));
	if(!rb_const_defined(mGPS, id_Longitude)) return 0;
	cLongitude = rb_const_get(mGPS, rb_intern("Longitude"));
	return 1;
}

/*
 * Usage: NMEA::scan(nmea_sentence, handler)
 */
static VALUE scan(VALUE self, VALUE sentence, VALUE handler) {
	Check_Type(sentence, T_STRING);
	char *ptr = RSTRING(sentence)->ptr;
	char last = ptr[RSTRING(sentence)->len-1];
	char ptr_copy[RSTRING(sentence)->len+2];
	if(last != '\n' && last != '\r') {
		memcpy(ptr_copy, ptr, sizeof(ptr_copy)-2);
		ptr_copy[sizeof(ptr_copy)-2] = '\n';
		ptr_copy[sizeof(ptr_copy)-1] = 0;
		ptr = ptr_copy;
	}
	nmea_scanner(ptr, handler);
	return Qnil;
}



void Init_nmea_bin() {
	id_GPS = rb_intern("GPS");
	id_Latitude = rb_intern("Latitude");
	id_Longitude = rb_intern("Longitude");
	id_new = rb_intern("new");
	id_rmc = rb_intern("rmc");
	id_gsv = rb_intern("gsv");
	id_psrftxt = rb_intern("psrftxt");
	id_vtg = rb_intern("vtg");
	id_start = ID2SYM(rb_intern("start"));
	id_continue = ID2SYM(rb_intern("continue"));
	id_finish = ID2SYM(rb_intern("finish"));
	id_gsa = rb_intern("gsa");
	id_manual = ID2SYM(rb_intern("manual"));
	id_automatic = ID2SYM(rb_intern("automatic"));
	id_no_fix = ID2SYM(rb_intern("no_fix"));
	id_2d = ID2SYM(rb_intern("gps_2d"));
	id_3d = ID2SYM(rb_intern("gps_3d"));
	id_autonomus = ID2SYM(rb_intern("autonomus"));
	id_differential = ID2SYM(rb_intern("differential"));
	id_estimated = ID2SYM(rb_intern("estimated"));
	id_invalid = ID2SYM(rb_intern("invalid"));
	id_gga = rb_intern("gga");
	id_gll = rb_intern("gll");
	cLatitude = Qnil;
	cLongitude = Qnil;
	cTime = rb_const_get(rb_cObject, rb_intern("Time"));
/*
 * Document-module: NMEA
 * NMEA module has the only method: scan. Better read about it.
 */
	mNMEA = rb_define_module("NMEA");
	rb_define_singleton_method(mNMEA, "scan", scan, 2);
/*
 * Document-class: NMEA::ParseError
 * You will receive ParseError if NMEA::scan cannot parse sentence. Usually it happens, when
 * you try to parse broken sentence. Perhaps, through it away.
 */
	eParseError = rb_define_class_under(mNMEA, "ParseError", rb_eStandardError);
	cSatelliteInfo = rb_struct_define(NULL, "number", "elevation", "azimuth", "signal_level", NULL);
	/* Struct.new(:number, :elevation, :azimuth, :signal_level): assigned to SatelliteInfo */
	rb_define_const(mNMEA, "SatelliteInfo", cSatelliteInfo);
}