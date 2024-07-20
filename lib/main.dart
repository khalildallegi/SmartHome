import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_database/firebase_database.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp();
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'LED Controller',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: MyHomePage(),
    );
  }
}

class MyHomePage extends StatefulWidget {
  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  final DatabaseReference _ledStatusRef =
      FirebaseDatabase.instance.ref().child('led').child('status');

  bool _ledStatus = false;

  @override
  void initState() {
    super.initState();
    _ledStatusRef.onValue.listen((event) {
      final bool newStatus = event.snapshot.value == 1;
      setState(() {
        _ledStatus = newStatus;
      });
    });
  }

  void _toggleLed() {
    final int newStatus = _ledStatus ? 0 : 1;
    _ledStatusRef.set(newStatus);
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('LED Controller'),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Text(
              'LED is ${_ledStatus ? "ON" : "OFF"}',
            ),
            SizedBox(height: 20),
            ElevatedButton(
              onPressed: _toggleLed,
              child: Text(_ledStatus ? 'Turn OFF' : 'Turn ON'),
            ),
          ],
        ),
      ),
    );
  }
}
