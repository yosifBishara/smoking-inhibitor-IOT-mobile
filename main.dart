// ignore: invalid_language_version_override
// dart=2.9
import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'login_page.dart';
import 'samples_page.dart';
import 'setTime.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp();

  runApp(MaterialApp(
    initialRoute: '/home',
    routes: {
      '/' : (context) => const LoginPage(),
      '/home' : (context) => const LoginPage(),
      '/samples' : (context) => const SamplesPage(),
      '/set_time' : (context) => const SetTime()
    },
  ));
}

