import de.bezier.data.sql.*;

SQLite db;
 
final int FRAME_RATE = 60;
final int EEG_RATE = 500;
final int NUMBER_OF_CHANNELS = 11;
final int WIDTH = 1024;
final int HEIGHT = 768;

int samplesPerFrame;
int samplesPerLastFrame;

ArrayList[] frameData = new ArrayList[NUMBER_OF_CHANNELS];

void setup() {
  size(WIDTH,HEIGHT);
  frameRate(FRAME_RATE);

  samplesPerFrame = EEG_RATE / FRAME_RATE;
  samplesPerLastFrame = EEG_RATE / FRAME_RATE + EEG_RATE % FRAME_RATE;

  println("Samples per frame: " + samplesPerFrame);
  println("Samples per last frame: " + samplesPerLastFrame);

  for (int i = 0; i < NUMBER_OF_CHANNELS; i++) {
    frameData[i] = new ArrayList();
  }

  db = new SQLite( this, "/Volumes/Store/Avner/POUYAN-BRAIN/pouyan_eeg.db"  );
  if ( db.connect() ) {
    println("Connected to database!");
    db.query( "SELECT * FROM data" );
  }
}

void draw() {
  db.next();
  for (int i = 0; i < NUMBER_OF_CHANNELS; i++) {
    frameData[i].clear();
  }
  int framesToRead = (frameCount % 60 == 0) ? samplesPerLastFrame : samplesPerFrame;

  for (int i = 0; i < framesToRead; i++) {
    // READ ALL COLUMNS
    /*
    for (int channel = 0; channel < NUMBER_OF_CHANNELS; channel++) {
      frameData[channel].add(db.getFloat(channel + 1));
    }*/
  
    // READ JUST LPP
    frameData[10].add(db.getFloat(11));
  }
  // Draw some circles
  for (int i = 0; i < frameData[10].size(); i++) {
    float value = (Float)(frameData[10].get(i)) * 1000000;
    ellipse(random(HEIGHT), random(WIDTH), value, value);
  }
}

