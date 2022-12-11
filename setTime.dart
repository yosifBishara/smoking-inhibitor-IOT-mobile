import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'fire_store.dart';
import 'login_page.dart';


class SetTime extends StatefulWidget {
  const SetTime({Key? key}) : super(key: key);

  @override
  _SetTime createState() => _SetTime();
}

class _SetTime extends State<SetTime> {

  final controlNum = TextEditingController();
  final _formKey = GlobalKey<FormState>();
  FireHelper db = FireHelper();
  String dropdownValue = 'min';
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      resizeToAvoidBottomInset: false,
      appBar: AppBar(
        title: const Text(
            ''
        ),
        backgroundColor: CupertinoColors.black,
        shadowColor: Colors.grey,
      ),
      body: Center(
        child: Container(
          width: MediaQuery.of(context).size.width * 0.55,
          height: MediaQuery.of(context).size.height * 0.5,
          child: Center(
            child: Form(
              key: _formKey,
              child: Column(
                children: <Widget>[
                  const Text(
                    'set new duration:',
                    style: TextStyle(
                      fontSize: 20,
                    ),
                  ),

                  SizedBox(height: MediaQuery.of(context).size.height * 0.015,),

                  TextFormField(
                    enableSuggestions: true,
                    // ignore: missing_return
                    validator: (value) {
                      if(controlNum.text.isEmpty) {
                        return 'insert time';
                      }
                      for (int i=0 ; i < controlNum.text.length ; i++) {
                        String ch = controlNum.text[i];
                        if ((ch.compareTo("0") < 0) || (ch.compareTo("9") > 0)) {
                          return 'invalid number';
                        }
                      }
                    },
                    maxLengthEnforced: true,
                    maxLength: 10,
                    keyboardType: TextInputType.number,
                    controller: controlNum,
                    key: const Key('phone number'),
                    textAlign: TextAlign.end,
                    decoration: const InputDecoration(
                      counterText: '',
                      focusedBorder: OutlineInputBorder(
                        borderSide: BorderSide(color: Colors.black, width: 2.5),
                      ),
                      enabledBorder: OutlineInputBorder(
                        borderSide: BorderSide(color: Colors.black, width: 2.5),
                      ),
                      hintText: '',
                      hintStyle: TextStyle(
                        color: Colors.black,
                      ),
                    ),
                    style: const TextStyle(
                      color: Colors.black,
                    ),
                  ),

                  DropdownButton<String>(
                    value: dropdownValue,
                    icon: const Icon(Icons.arrow_downward),
                    elevation: 16,
                    style: const TextStyle(color: Colors.deepPurple),
                    underline: Container(
                      height: 2,
                      color: Colors.deepPurpleAccent,
                    ),
                    onChanged: (String? newValue) {
                      setState(() {
                        dropdownValue = newValue!;
                      });
                    },
                    items: <String>['min', 'hour', 'day']
                        .map<DropdownMenuItem<String>>((String value) {
                      return DropdownMenuItem<String>(
                        value: value,
                        child: Text(value),
                      );
                    }).toList(),
                  ),

                  Padding(
                    padding: const EdgeInsets.all(8.0),
                    child: TextButton(
                      onPressed: () {
                        if(_formKey.currentState!.validate()) {
                          if (controlNum.text.isNotEmpty) {
                            int duration = int.parse(controlNum.text);
                            if (dropdownValue.compareTo('min') == 0) { duration *= 60; }
                            if (dropdownValue.compareTo('hour') == 0) { duration *= (60*60); }
                            if (dropdownValue.compareTo('day') == 0) { duration *= (24*60*60); }
                            db.fireStoreInstance.collection('Users').doc(caseId).update({
                              'duration_seconds': duration
                            });
                            Navigator.pop(context);
                          }
                        }
                      }, // change this to real function
                      child: const Padding(
                        padding: EdgeInsets.all(8.0),
                        child: Text(
                          'set',
                          style: TextStyle(
                            fontSize: 20,
                            color: Colors.black,
                          ),
                        ),
                      ),

                    ),
                  ),

                ],
              ),
            ),
          ),

        ),

      ),
    );
  }
}