import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'utilities/helpful_classes.dart';
import 'utilities/Authentication.dart';
import 'package:provider/provider.dart';
import 'package:flutter/material.dart';
import 'utilities/ImageInfoPage.dart';
import 'utilities/helpful_classes.dart';
import 'package:firebase_core/firebase_core.dart';
import 'dart:collection';
import 'package:tuple/tuple.dart';

import 'package:firebase_storage/firebase_storage.dart' as firebase_storage;



Future<SplayTreeMap<DateTime, Tuple2<String,String>>> getPicPaths(String fileName) async {
  firebase_storage.ListResult result =
  await firebase_storage.FirebaseStorage.instance.ref().child(fileName).listAll();

  SplayTreeMap<DateTime, Tuple2<String,String>> image_dates_and_paths = SplayTreeMap<DateTime, Tuple2<String,String>>();

  await Future.forEach(result.items, (firebase_storage.Reference ref) async {
    String imageUrl = await ref.getDownloadURL();
    firebase_storage.FullMetadata file_data = await ref.getMetadata();
    DateTime file_date = file_data.updated!;
    String file_title = file_data.name;
    image_dates_and_paths[file_date] = Tuple2(file_title, imageUrl);
    print('Found file: $ref');
  });

  result.prefixes.forEach((firebase_storage.Reference ref) {
    print('Found directory: $ref');
  });

  return image_dates_and_paths;
}


Future<List<ImageDetails>> getImages(BuildContext context) async {
  String email = Provider.of<AppUser>(context, listen: false).user!.email!;
  String emailName = email.substring(0, email.indexOf("@"));
  String userImagesFileName = "robot_" + emailName + "_footage";
  SplayTreeMap<DateTime, Tuple2<String,String>> imageDatesAndNames = await getPicPaths(userImagesFileName);
  List<ImageDetails> image_details = List.generate(0, (index) => ImageDetails(imagePath: "", date: DateTime.now(), title: "", notes: ""));
  imageDatesAndNames.forEach((key, value) {
    /*
    try{
      String dateSubstr = imageDatesAndNames[i].substring(imageDatesAndNames[i].indexOf(userImagesFileName) + userImagesFileName.length + 3, imageDatesAndNames[i].indexOf(userImagesFileName) + userImagesFileName.length + 22);
      dateSubstr = dateSubstr.substring(0, 10) + " " + dateSubstr.substring(13);
      DateTime image_date = DateTime.parse(dateSubstr);
      image_details.add(ImageDetails(imagePath: imageDatesAndNames[i], date: image_date, title: "", notes: ""));
    }
    catch(e){
      print("Unknown Date picture name");
      image_details.add(ImageDetails(imagePath: imageDatesAndNames[i], date: DateTime(2000), title: "", notes: ""));
    }
    */
    image_details.add(ImageDetails(imagePath: value.item2, date: key, title: value.item1, notes: ""));

  });
  return image_details;
}

/*
List<ImageDetails> _images = [
  ImageDetails(
    imagePath: 'images/pic1.jpeg',
    date: DateTime(2021, 1, 15, 12, 52, 31),
    title: 'Casual Look',
    notes:
    'Lorem ipsum dolor sit amet, consectetur adipisicing elit. Nihil error aspernatur, sequi inventore eligendi vitae dolorem animi suscipit. Nobis, cumque.',
  ),
  ImageDetails(
    imagePath: 'images/pic6.jpg',
    date: DateTime(2022, 5, 15, 12, 52, 31),
    title: 'Help',
    notes:
    'Lorem ipsum dolor sit amet, consectetur adipisicing elit. Nihil error aspernatur, sequi inventore eligendi vitae dolorem animi suscipit. Nobis, cumque.',
  ),
  ImageDetails(
    imagePath: 'images/pic11.jpg',
    date: DateTime(2022, 11, 15, 12, 52, 31),
    title: 'New York',
    notes:
    'Steph!',
  ),
  ImageDetails(
    imagePath: 'images/pic11.jpg',
    date: DateTime(2022, 11, 15, 12, 52, 31),
    title: 'New York',
    notes:
    'Steph!',
  ),
  ImageDetails(
    imagePath: 'images/pic11.jpg',
    date: DateTime(2022, 11, 15, 12, 52, 31),
    title: 'New York',
    notes:
    'Steph!',
  ),
  ImageDetails(
    imagePath: 'images/pic1.jpeg',
    date: DateTime(2021, 1, 15, 12, 52, 31),
    title: 'Casual Look',
    notes:
    'Lorem ipsum dolor sit amet, consectetur adipisicing elit. Nihil error aspernatur, sequi inventore eligendi vitae dolorem animi suscipit. Nobis, cumque.',
  ),
  ImageDetails(
    imagePath: 'images/pic6.jpg',
    date: DateTime(2022, 5, 15, 12, 52, 31),
    title: 'Help',
    notes:
    'Lorem ipsum dolor sit amet, consectetur adipisicing elit. Nihil error aspernatur, sequi inventore eligendi vitae dolorem animi suscipit. Nobis, cumque.',
  ),
  ImageDetails(
    imagePath: 'images/pic11.jpg',
    date: DateTime(2022, 11, 15, 12, 52, 31),
    title: 'New York',
    notes:
    'Steph!',
  ),
  ImageDetails(
    imagePath: 'images/pic11.jpg',
    date: DateTime(2022, 11, 15, 12, 52, 31),
    title: 'New York',
    notes:
    'Steph!',
  ),
  ImageDetails(
    imagePath: 'images/pic11.jpg',
    date: DateTime(2022, 11, 15, 12, 52, 31),
    title: 'New York',
    notes:
    'Steph!',
  ),
  ImageDetails(
    imagePath: 'images/pic1.jpeg',
    date: DateTime(2021, 1, 15, 12, 52, 31),
    title: 'Casual Look',
    notes:
    'Lorem ipsum dolor sit amet, consectetur adipisicing elit. Nihil error aspernatur, sequi inventore eligendi vitae dolorem animi suscipit. Nobis, cumque.',
  ),
  ImageDetails(
    imagePath: 'images/pic6.jpg',
    date: DateTime(2022, 5, 15, 12, 52, 31),
    title: 'Help',
    notes:
    'Lorem ipsum dolor sit amet, consectetur adipisicing elit. Nihil error aspernatur, sequi inventore eligendi vitae dolorem animi suscipit. Nobis, cumque.',
  ),
  ImageDetails(
    imagePath: 'images/pic11.jpg',
    date: DateTime(2022, 11, 15, 12, 52, 31),
    title: 'New York',
    notes:
    'Steph!',
  ),
  ImageDetails(
    imagePath: 'images/pic11.jpg',
    date: DateTime(2022, 11, 15, 12, 52, 31),
    title: 'New York',
    notes:
    'Steph!',
  ),
  ImageDetails(
    imagePath: 'images/pic11.jpg',
    date: DateTime(2022, 11, 15, 12, 52, 31),
    title: 'New York',
    notes:
    'Steph!',
  ),
];
*/

class ImageDetails {
  final String imagePath;
  final DateTime date;
  final String title;
  final String notes;
  ImageDetails({
    required this.imagePath,
    required this.date,
    required this.title,
    required this.notes,
  });
}

class SavedFootagePage extends StatefulWidget {
  const SavedFootagePage({Key? key}) : super(key: key);

  @override
  _SavedFootagePageState createState() => _SavedFootagePageState();
}

class _SavedFootagePageState extends State<SavedFootagePage> {
  List<ImageDetails> _images = List.generate(0, (index) => ImageDetails(imagePath: "", date: DateTime.now(), title: "", notes: ""));
  @override
  void initState() {
    super.initState();
    WidgetsBinding.instance!.addPostFrameCallback((timeStamp) {
      getImages(context).then((value) => setState(() {_images = value;}));
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        // Here we take the value from the MyHomePage object that was created by
        // the App.build method, and use it to set our appbar title.
        title: Text("Saved Footage",
          style: TextStyle(color: Colors.green.shade900),
        ),

      ),
      body:
      SafeArea(
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: <Widget>[
            SizedBox(
              height: 40,
            ),
            Expanded(
              child: Container(
                padding: EdgeInsets.symmetric(
                  horizontal: 20,
                  vertical: 30,
                ),
                decoration: BoxDecoration(
                  color: Colors.white,
                  borderRadius: BorderRadius.only(
                    topLeft: Radius.circular(30),
                    topRight: Radius.circular(30),
                  ),
                ),
                child: (_images.length > 0) ? GridView.builder(
                  gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
                    crossAxisCount: 3,
                    crossAxisSpacing: 10,
                    mainAxisSpacing: MediaQuery.of(context).size.height * 0.01,

                  ),
                  itemBuilder: (context, index) {
                    return Column(
                      children: [
                        Container(
                          child: Text(
                            getCleanDateStr(_images[index].date),
                            style: TextStyle(
                              fontSize: 14,
                            ),
                          ),
                        ),
                        Container(
                          height: MediaQuery.of(context).size.height * 0.09,
                          child: RawMaterialButton(
                            onPressed: () {
                              Navigator.push(
                                context,
                                MaterialPageRoute(
                                  builder: (context) => DetailsPage(
                                    imagePath: _images[index].imagePath,
                                    date: _images[index].date,
                                    title: _images[index].title,
                                    notes: _images[index].notes,
                                    index: index,
                                  ),
                                ),
                              );
                            },
                            child: Hero(
                              tag: 'logo$index',
                              child: Container(
                                child: ClipRRect(
                                    borderRadius: BorderRadius.circular(12.0),
                                    child: Image.network(_images[index].imagePath)
                                ),
                              ),
                            ),
                          ),
                        ),
                      ],
                    );
                  },
                  itemCount: _images.length,
                ):
                Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    Text("no saved footage yet..."),
                  ],
                ),
              ),
            )
          ],
        ),
      ),
    );
  }
}
