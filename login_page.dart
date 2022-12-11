// ignore: invalid_language_version_override
// dart=2.9
import 'package:flutter/material.dart';

String caseId = '';

class LoginPage extends StatefulWidget {
  const LoginPage({Key? key}) : super(key: key);

  @override
  _LoginPageState createState() => _LoginPageState();
}

class _LoginPageState extends State<LoginPage> {

  final _formKey = GlobalKey<FormState>();
  final _textFieldController = TextEditingController();

  bool legalMacChar(String macChar) {
    return ((macChar.compareTo("A") >= 0) && (macChar.compareTo("F") <= 0)) ||
           ((macChar.compareTo("a") >= 0) && (macChar.compareTo("f") <= 0)) ||
           ((macChar.compareTo("0") >= 0) && (macChar.compareTo("9") <= 0));
  }

  bool legalMacAddress (String macAddress) {
    if (macAddress.length != 12) {
      return false;
    }
    for (int i=0 ; i < macAddress.length ; i++) {
      if (!legalMacChar(macAddress[i])) {
        return false;
      }
    }
    return true;
  }
  
  @override
  Widget build(BuildContext context) {
    return SafeArea(
        child: Scaffold (
          resizeToAvoidBottomInset: false,
          backgroundColor: Colors.grey[800],
          body: Form(
            key: _formKey,
            child: Center(
              child: Column(
                mainAxisAlignment: MainAxisAlignment.spaceAround,
                children: [

                  // title
                  const Text(
                    "Smoking Inhibitor IOT",
                    style: TextStyle(
                      fontSize: 30,
                      color: Colors.white
                    ),
                  ),

                  // login case number
                  SizedBox(
                    width: 270,
                    child: TextFormField(

                      enableSuggestions: true,
                      validator: (String? input) {
                        if (input!=null && input.isEmpty) return 'Enter A Case Number';
                        if (input!=null && !legalMacAddress(input)) return 'Invalid Case Number';
                      },
                      controller: _textFieldController,
                      key: const Key('Case Number'),
                      maxLengthEnforced: true,
                      maxLength: 17,
                      textAlign: TextAlign.start,
                      decoration: const InputDecoration(
                        focusedBorder: OutlineInputBorder(
                          borderSide: BorderSide(color: Colors.white60, width: 2.5),
                        ),
                        enabledBorder: OutlineInputBorder(
                          borderSide: BorderSide(color: Colors.black, width: 2.5),
                        ),
                        hintText: 'Case Number',
                        hintStyle: TextStyle(
                          color: Colors.white,
                        ),
                      ),
                      style: const TextStyle(
                        color: Colors.white,
                      ),
                    ),
                  ),

                  // login button
                  TextButton(

                      onPressed: ()  {
                        if (_formKey.currentState!.validate()) {
                          // setState(() {
                          //   caseId = _textFieldController.text;
                          // });
                          caseId = _textFieldController.text;
                          // ignore: avoid_print
                          print("Case ID = " + caseId);
                          Navigator.pushNamed(context, '/samples');
                        }
                      },
                      child: const Center(
                        child: Text(
                          "Sign In",
                          style: TextStyle(
                            color: Colors.white,
                            fontSize: 19
                          ),
                        )
                      )
                  )

                ],
              ),
            ),
          ),

        )
    );
  }
}
