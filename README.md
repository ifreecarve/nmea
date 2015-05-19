NMEA Ruby Gem
=============

* Author:    [Max Lapshin](mailto:max@maxidoors.ru)
* Copyright: Copyright (c) 2007 Max Lapshin, Getalime
* License:   Distributes under the same terms as Ruby



Usage
-----

To use NMEA parser, you should somehow get data stream from Your gps device.
For example, You can use ruby-serialport (which seems to be dead, but working).
The example on Mac with installed USB-Serial driver:

```ruby
	require 'serialport'
	require 'nmea'
	@sp = SerialPort.open("/dev/tty.usbserial", 4800, 8, 1, SerialPort::NONE)
	@handler = NMEAHandler.new
	while(@sentence = @sp.gets) do
	  NMEA.scan(@sentence, @handler)
	end
```

NMEAHandler is a user class, that implements following interface:

```ruby
	class NMEAHandler
	  def rmc(time, latitude, longitude, speed, course, magnetic_variation)
	    # read further about types of latitude and longitude
	  end
	  def gsv(flag, satellites)
	  end
	  def gsa(mode_state, mode, satellites, pdop, hdop, vdop)
	  end
	  def gga(time, latitude, longitude, gps_quality, active_satellite_count, gsa_hdop, altitude, geoidal_height, dgps_data_age, dgps_station_id)
	  end
	end
```

The following NMEA sentences are supported currently:

 * $GPRMC
 * $GPGSV
 * $GPGSA
 * $GPGGA

If you need more, please contact me and provide me with several examples. I will add support for them.

`NMEA::scan` will not try to call unexistent method on your handler, thus you can implement only those
methods in your handler, you need to.

GSV handler has parameter flag. This flag can take one of the following values: `:start`, `:continue`
 and `:finish`. GSV messages appears in packs of several sentences, but `NMEA::scan` is stateless,
thus your NMEAHandler should keep accumulate these messages.

It is important to mention, that `NMEA::scan` assumes that you have classes `GPS::Latitude` and `GPS::Longitude`, which initializer, takes two arguments: degrees and minutes. You can use this example:

```ruby
	module GPS
	  AngleValue = Struct.new :degrees, :minutes

	  class AngleValue
	    def to_s
	      "%d %.4f%s" % [degrees.abs, minutes, symbol]
	    end
	  end

	  class Latitude < AngleValue
	    def symbol
	      degrees >= 0 ? "N" : "S"
	    end
	  end
	  class Longitude < AngleValue
	    def symbol
	      degrees >= 0 ? "E" : "W"
	    end
	  end
	end
```


Supported devices
-----------------

Ruby NMEA has full support of [Globalsat BU-353 device](http://www.usglobalsat.com/item.asp?itemid=60&catid=17).

NMEA syntax is assumed to be as in document nmea.html, taken from http://www.werple.net.au/~gnb/gps/nmea.html