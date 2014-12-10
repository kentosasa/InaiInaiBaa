#include "testApp.h"

using namespace ofxCv;

void testApp::setup() {
    //画面基本設定
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    
    //カメラを初期化
    cam.initGrabber(640, 480);
    
    //初期データの合成するイメージのメモリ領域を確保して、読込み
    faceImage.allocate(640, 480, OF_IMAGE_COLOR);
    faceImage.loadImage("stevejobs.jpg");
    
    //配列で画像を読み込む
    for (int i = 0; i < 4; i++) {
        switch (i) {
            case 0:
                faceImages[0].loadImage("kenshiro.jpg");
                break;
            case 1:
                faceImages[1].loadImage("monariza.jpg");
                break;
            case 2:
                faceImages[2].loadImage("stevejobs.jpg");
                break;
            case 3:
                faceImages[3].loadImage("aragaki.jpg");
                break;
            default:
                break;
        }
    }
    //カメラ映像のフェイストラッカーのセットアップ
    tracker.setup();
    //合成する顔画像のフェイストラッカーのセットアップ
    imgTracker.setup();
    
    //音声読み込み
    inaiinai.loadSound("inaiinai.mp3");
    baa.loadSound("baa.mp3");
}

void testApp::update() {
    //カメラ更新
    cam.update();
    if(cam.isFrameNew()) {
        //フェイストラッカーの更新
        imgTracker.update(toCv(faceImage));
        tracker.update(toCv(cam));
    }
}

void testApp::draw() {
    //カメラ映像を描画
    ofSetColor(255);
    cam.draw(0, 0);
    //フレームレート表示
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
    
    //もしカメラの映像に顔が検出されたら以下の処理をする
    if(tracker.getFound()) {
        //音声再生をする
        inaiinaiFlag = true;
        if (baaFlag) {
            baa.setPosition(0.0f);
            baa.play();
            baaFlag = false;
        }
        
        //カメラ映像からメッシュを作成
        ofMesh objectMesh = tracker.getObjectMesh();
        //合成する顔の画像からメッシュを作成
        ofMesh imgMesh = imgTracker.getObjectMesh();
        
        //静止画のメッシュの頂点情報を、カメラから生成したメッシュのものに変換
        //つまり現在の顔の表情を、静止画のメッシュに適用
        int limit = objectMesh.getNumVertices();
        if (objectMesh.getNumVertices() > imgMesh.getNumVertices()) {
            limit = imgMesh.getNumVertices();
        }
        for (int i = 0; i < limit; i++) {
            imgMesh.setVertex(i, objectMesh.getVertex(i));
        }
        
        //カメラで検出された顔の、位置、大きさ、傾きを取得
        ofVec2f positon = tracker.getPosition();
        float scale = tracker.getScale();
        ofVec3f orientation = tracker.getOrientation();
        
        //静止画のメッシュをカメラの位置、大きさ、傾きにあわせる
        ofPushMatrix();
        ofTranslate(positon.x, positon.y);
        ofScale(scale, scale, scale);
        ofRotateX(orientation.x * 45.0f);
        ofRotateY(orientation.y * 45.0f);
        ofRotateZ(orientation.z * 45.0f);
        
        //静止画から生成メッシュを配置して、合成する画像をマッピング
        ofSetColor(255, 255, 255, 255);
        faceImage.getTextureReference().bind();
        imgMesh.draw();
        faceImage.getTextureReference().unbind();
        ofPopMatrix();
    }else{
        //ランダムで画像を入れ替える
        faceImage = faceImages[(int)ofRandom(0,3)];
        
        //「いないいない」の音声を流す
        if (inaiinaiFlag) {
            inaiinai.setPosition(0.0f);
            inaiinai.play();
            inaiinaiFlag = false;
            baaFlag = true;
        }
    }
}

void testApp::keyPressed(int key) {
//    //トラッカーのリセット
//    if(key == 'r') {
//        tracker.reset();
//        imgTracker.reset();
//    }
}