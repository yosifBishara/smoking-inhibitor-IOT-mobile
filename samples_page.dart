// ignore: invalid_language_version_override
// dart=2.9
// ignore: import_of_legacy_library_into_null_safe
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'fire_store.dart';
import 'login_page.dart';

class SamplesPage extends StatefulWidget {
  const SamplesPage({Key? key}) : super(key: key);

  @override
  _SamplesPageState createState() => _SamplesPageState();
}

class _SamplesPageState extends State<SamplesPage> {

  final FireHelper _fireHelper = FireHelper();
  DateTime now = DateTime.now();
  List weekdays = ['monday', 'tuesday', 'wednesday', 'thursday', 'friday', 'saturday', 'sunday'];

  void showNoDataAlert(BuildContext context) async {
    showDialog<String>(
        context: context,
        builder: (BuildContext buildContext) {
          return AlertDialog(
            title: const Text("No Data Found!"),
            content: const Text("Looks like you did not use your case"),
            actions: [
              TextButton(
                  onPressed: () => Navigator.pop(context, 'Got it'),
                  child: const Text("Got it")
              )
            ],
            actionsAlignment: MainAxisAlignment.center,
          );
        }
    );
  }

  String dropDownValue = 'One';

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      resizeToAvoidBottomInset: false,
      backgroundColor: Colors.grey[800],
      appBar: AppBar(
        backgroundColor: Colors.black,
        automaticallyImplyLeading: false,
        title: Center(
          child: Text("${now.day}/${now.month}/${now.year} - ${weekdays[now.weekday-1]}"),
        ),
        actions: [
          IconButton(
              onPressed: () {
                Navigator.pushNamed(context, '/set_time');
              },
              icon: const Icon(Icons.alarm)
          )
        ],
      ),
      
      body: StreamBuilder<QuerySnapshot>(

        stream: _fireHelper.fireStoreInstance.collection('Users/$caseId/Samples').where(
            "date", isEqualTo: "${now.day}/${now.month}/${now.year}"
        ).snapshots(),

        builder: (BuildContext context, AsyncSnapshot<QuerySnapshot> snapshot) {
          if (snapshot.hasData == false) {
            return const Center();
          } else {
            List<QueryDocumentSnapshot> myDocs = snapshot.data!.docs;
            // if (myDocs.isEmpty) { showNoDataAlert(context); }
            myDocs.sort((a, b) {
              String x = a.get('time').toString();
              var y = a.get('time').toString().split(':');
              int aHour = int.parse(a.get('time').toString().split(':')[0]),
                  aMin = int.parse(a.get('time').toString().split(':')[1]),
                  bHour = int.parse(b.get('time').toString().split(':')[0]),
                  bMin = int.parse(b.get('time').toString().split(':')[1]);
              // ignore: avoid_print
              print(myDocs.length);
              return aHour - bHour == 0 ? aMin - bMin : aHour - bHour;
            });

            return ListView(
              children: myDocs.map((document) {
                String cardContent = "BPM = ${document.get('BPM')}\n";
                cardContent += "Movement"
                    " ${document.get('Acceleration') > 5 ? 'detected' : 'not detected'}\n";
                cardContent += "Air Pollution = " + ((document.get('TVOC') > 500 || document.get('eCO2') > 1000 )? "High\n" : "low\n");
                cardContent += "time = " + document.get('time') + "\n";
                cardContent += "cigarette = ${document.get('cigarette')}\n";
                return Card(
                  shadowColor: Colors.black,
                  elevation: 10,
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(MediaQuery.of(context).size.width*0.015)
                  ),
                  child: Padding(
                    padding: const EdgeInsets.fromLTRB(0, 2, 8, 2),
                    child: Text(
                      cardContent,
                      textDirection: TextDirection.ltr,
                      style: const TextStyle(
                          fontSize: 15
                      ),
                    ),
                  )
                );
              }).toList(),
            );
          }
        },
      ),
    );
  }
}
