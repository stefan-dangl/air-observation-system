import pytest
from serial_interface import _parse_sensor_data


def test_valid_data_parsing():
    data_str = "2025-01-01T12:00:00;42;450.5;0.75"
    expected = {
        "timestamp": "2025-01-01T12:00:00",
        "sensor_id": 42,
        "CO2 [ppm]": 450.5,
        "CH4 [% LEL]": 0.75
    }
    assert _parse_sensor_data(data_str) == expected


def test_whitespace_handling():
    data_str = "  2025-01-01T12:00:00; 42; 450.5; 0.75  \n"
    result = _parse_sensor_data(data_str)
    assert result["timestamp"] == "2025-01-01T12:00:00"


@pytest.mark.parametrize("data_str", [
    "2025-01-01T12:00:00;abc;450.5;0.75",       # Non-numeric sensor_id
    "2025-01-01T12:00:00;42;invalid;0.75",      # Non-numeric CO2 value
    "2025-01-01T12:00:00;42;450.5;invalid",     # Non-numeric CH4 value
])
def test_wrong_segment_type(data_str):
    assert _parse_sensor_data(data_str) is None


@pytest.mark.parametrize("data_str", [
    "2025-01-01T12:00:00;42;450.5;0.75;extra",  # Too many segments
    "2025-01-01T12:00:00;42;450.5",             # Too few segments
    ";;;;",                                     # Empty segments
    None,                                       # None input
    "",                                         # Empty input
])
def test_invalid_formats(data_str):
    assert _parse_sensor_data(data_str) is None
