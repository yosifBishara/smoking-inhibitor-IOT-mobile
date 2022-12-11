// ignore: invalid_language_version_override
// dart=2.9
class Sample {
    String _id, _date, _time;
    int _acceleration, _bpm, _tvoc, _eco2;
    bool _cigarette;


    Sample(this._id, this._acceleration, this._bpm, this._tvoc, this._eco2, this._date, this._time, this._cigarette);

    String get id => _id;
    String get date => _date;
    String get time => _time;
    int get acceleration => _acceleration;
    int get BPM => _bpm;
    int get TVOC => _tvoc;
    int get eCO2 => _eco2;
    bool get cigarette => _cigarette;

    Map<String, dynamic> toMap() {
      var map = <String, dynamic>{};
      map['date'] = _date;
      map['time'] = _time;
      map['acceleration'] = _acceleration;
      map['BPM'] = _bpm;
      map['TVOC'] = _tvoc;
      map['eCO2'] = _eco2;
      map['cigarette'] = _cigarette;
      return map;
    }

    void fromMap(Map<String, dynamic> map) {
      _date = map['date'];
      _time = map['time'];
      _acceleration = map['acceleration'];
      _bpm = map['BPM'];
      _tvoc = map['TVOC'];
      _eco2 = map['eCO2'];
      _cigarette = map['cigarette'];
    }

}