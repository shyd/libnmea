#include "gpgga.h"

int
nmea_gpgga_init(nmea_sentence_parser_s *parser)
{
	memset(parser, 0, sizeof(nmea_sentence_parser_s));
	parser->parse = &nmea_gpgga_parse;

	/* Allocate data struct */
	nmea_s *data;
	data = malloc(sizeof(nmea_gpgga_s));
	if (NULL == data) {
		return -1;
	}
	memset(data, 0, sizeof(nmea_gpgga_s));
	parser->data = data;

	return 0;
}

int
nmea_gpgga_parse(char **values, int length, nmea_s *nmea_data)
{
	nmea_gpgga_s *data = (nmea_gpgga_s *) nmea_data;

	int i = 0;
	char *value;
	while (i < length) {
		value = values[i];
		if (-1 == nmea_value_is_set(value)) {
			i++;
			continue;
		}

		switch (i) {
			case NMEA_GPGGA_TIME:
				/* Parse time */
				if (-1 == nmea_time_parse(value, &data->time)) {
					nmea_data->error = 1;
				}
				break;

			case NMEA_GPGGA_LATITUDE:
				/* Parse latitude */
				if (-1 == nmea_position_parse(value, &data->latitude)) {
					nmea_data->error = 1;
				}
				break;

			case NMEA_GPGGA_LATITUDE_CARDINAL:
				/* Parse cardinal direction */
				data->latitude.cardinal = nmea_cardinal_direction_parse(value);
				if (NMEA_CARDINAL_DIR_UNKNOWN == data->latitude.cardinal) {
					nmea_data->error = 1;
				}
				break;

			case NMEA_GPGGA_LONGITUDE:
				/* Parse longitude */
				if (-1 == nmea_position_parse(value, &data->longitude)) {
					nmea_data->error = 1;
				}
				break;

			case NMEA_GPGGA_LONGITUDE_CARDINAL:
				/* Parse cardinal direction */
				data->longitude.cardinal = nmea_cardinal_direction_parse(value);
				if (NMEA_CARDINAL_DIR_UNKNOWN == data->longitude.cardinal) {
					nmea_data->error = 1;
				}
				break;

			case NMEA_GPGGA_N_SATELLITES:
				/* Parse number of satellies */
				data->n_satellites = atoi(value);
				break;

			case NMEA_GPGGA_ALTITUDE:
				/* Parse altitude */
				data->altitude = atoi(value);
				break;

			case NMEA_GPGGA_ALTITUDE_UNIT:
				/* Parse altitude unit */
				data->altitude_unit = *value;
				break;

			default:
				break;
		}

		i++;
	}

	return 0;
}
