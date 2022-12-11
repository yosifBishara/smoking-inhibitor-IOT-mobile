// ignore: invalid_language_version_override
// dart=2.9
import 'dart:async';
// ignore: import_of_legacy_library_into_null_safe
import 'package:cloud_firestore/cloud_firestore.dart';




class FireHelper {

  static final FirebaseFirestore _firestore = FirebaseFirestore.instance;
  // final CollectionReference collectionReference = FirebaseFirestore.instance.collection('appointments');
  DateTime now = DateTime.now();


  FireHelper();

  FirebaseFirestore get fireStoreInstance => _firestore;

  Future<bool> caseExistInDB (String caseID) async {
    try {
      var caseDoc = await _firestore.collection('Users').doc(caseID).get();
      return caseDoc.exists;
    } catch (e) {
      return false;
    }
  }

  // Future<bool> addApp(Sample sample) async {
  //   try {
  //     await _firestore.collection('appointments').doc(sample.id).set({
  //       'name' : appointment.name,
  //       'date' : appointment.date,
  //       'day' : appointment.day,
  //       'persons' : appointment.persons,
  //       'time' : appointment.time
  //     });
  //     return true;
  //   } catch(e) {
  //     return false;
  //   }
  // }

  // Future<Appointment> getApp(String phone_num) async {
  //   DocumentSnapshot documentSnapshot;
  //   try {
  //     documentSnapshot = await _firestore.collection('appointments').doc(phone_num).get();
  //     Appointment app = Appointment(documentSnapshot.get('name'),
  //         phone_num, documentSnapshot.get('date'),
  //         documentSnapshot.get('day'), documentSnapshot.get('persons'),
  //         documentSnapshot.get('time'));
  //     return app;
  //
  //   } catch(e) {
  //     return null;
  //   }
  //
  // }

  // Future<bool> updateApp(Appointment appointment) async {
  //   try {
  //     await _firestore.collection('appointments').doc(appointment.number).update({
  //       'name' : appointment.name,
  //       'date' : appointment.date,
  //       'day' : appointment.day,
  //       'persons' : appointment.persons,
  //       'time' : appointment.time
  //     });
  //     return true;
  //   }catch(e) {
  //     return false;
  //   }
  // }
  //
  // Future<bool> deleteApp(String phone_num) async {
  //   try {
  //     await _firestore.collection('appointments').doc(phone_num).delete();
  //     return true;
  //   } catch (e) {
  //     return false;
  //   }
  // }
  //
  // //check if a given date and time are appointed - for time dropDown
  // Future<bool> takenDateTime(String date, String time) async {
  //   QuerySnapshot res = await _firestore.collection('appointments').get();
  //   List<DocumentSnapshot> docs = res.docs;
  //   List<dynamic> timeList = List();
  //   for(int i=0 ; i < docs.length ; i++){
  //     timeList = docs[i].get('time');
  //     if(docs[i].get('date')==date){
  //       for(int j =0 ; j < timeList.length ; j++){
  //         }
  //       }
  //     }
  //   }
  //   return false;
  // }
  //
  // //check if a person already has an appointment
  // Future<bool> personAlreadyAppointed(String personNumber) async {
  //   QuerySnapshot res = await _firestore.collection('appointments').get();
  //   List<DocumentSnapshot> docs = res.docs;
  //   for(int i=0 ; i < docs.length ; i++){
  //     if(docs[i].id == personNumber){
  //       return true;
  //     }
  //   }
  //   return false;
  // }
  //
  // Future<DocumentSnapshot?> returnAppointmentData(String personNumber) async {
  //   QuerySnapshot res = await _firestore.collection('appointments').get();
  //   List<DocumentSnapshot> docs = res.docs;
  //   for(int i = 0 ; i < docs.length ; i++){
  //     if(docs[i].id == personNumber)
  //       return docs[i];
  //   }
  //   return null;
  // }
  //
  // Future<int> initSelectedNum() async{
  //   QuerySnapshot res = await _firestore.collection('appointments').where('date', isEqualTo: '${today.day}/${today.month}/${today.year}').get();
  //   List<DocumentSnapshot> docs = res.docs;
  //   return docs.length;
  // }

}