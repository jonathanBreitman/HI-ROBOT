import 'package:flutter/material.dart';
import 'package:hr_app/utilities/helpful_classes.dart';
import 'package:hr_app/utilities/lib.dart' as lib;

class DetailsPage extends StatelessWidget {
  final String imagePath;
  final DateTime date;
  final String title;
  final String notes;
  final int index;
  DetailsPage(
      { required this.imagePath,
        required this.date,
        required this.title,
        required this.notes,
        required this.index});
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Container(
        decoration: lib.gradientBG,
        child: Column(
          children: <Widget>[
            Expanded(
              child: Container(
                height: MediaQuery.of(context).size.height * 0.8,
                child: Hero(
                  tag: 'logo$index',
                  child: Container(
                    child: ClipRRect(
                        borderRadius: BorderRadius.circular(5.0),
                        child: Image.network(imagePath)
                    ),
                  ),
                ),
              ),
            ),
            Container(
              height: MediaQuery.of(context).size.height * 0.2,
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: <Widget>[
                  Padding(
                    padding: EdgeInsets.fromLTRB(10, 10, 10, 0),
                    child: Container(
                      height: MediaQuery.of(context).size.height * 0.12,
                      child: SingleChildScrollView(
                        child: Column(
                          crossAxisAlignment: CrossAxisAlignment.start,
                          children: <Widget>[
                            Text(
                              title,
                              style: TextStyle(
                                color: Colors.white,
                                fontSize: 22,
                                fontWeight: FontWeight.w600,
                              ),
                            ),
                            SizedBox(
                              height: MediaQuery.of(context).size.height * 0.005,
                            ),
                            Text(
                              getCleanDateStr(date),
                              style: TextStyle(
                                color: Colors.white,
                                fontSize: 18,
                                fontWeight: FontWeight.w400,
                              ),
                            ),
                            SizedBox(
                              height: MediaQuery.of(context).size.height * 0.01,
                            ),
                            Text(
                              notes,
                              //overflow: TextOverflow.ellipsis,
                              style: TextStyle(
                                fontSize: 14,
                              ),
                            ),
                          ],
                        ),
                      ),
                    ),
                  ),
                  Container(
                    height: MediaQuery.of(context).size.height * 0.06,
                    width: MediaQuery.of(context).size.width,
                    child: ElevatedButton(
                      onPressed: () {
                        Navigator.pop(context);
                      },
                      style: ButtonStyle(
                        padding: MaterialStateProperty.all<EdgeInsets>(
                            EdgeInsets.all(15)),
                        foregroundColor:MaterialStateProperty.all<Color>(
                            Colors.white),
                        backgroundColor:
                        MaterialStateProperty.all<Color>(
                            lib.btnColor),
                      ),

                      child: Text(
                        'Back',
                        style: TextStyle(
                          color: Colors.white,
                        ),
                      ),
                    ),
                  )
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }
}