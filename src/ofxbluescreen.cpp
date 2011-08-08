#include "ofxbluescreen.h"

ofxBluescreen::ofxBluescreen():threshhold(50)
{
	bgColor.set(0, 0, 255);
}

ofxBluescreen::~ofxBluescreen()
{
}

void ofxBluescreen::learnBgColor(ofPixelsRef pixelSource)
{
	int wXh = pixelSource.getWidth() * pixelSource.getHeight();
	int numPixels = wXh * pixelSource.getBytesPerPixel();
	int r,g,b;
	r=g=b=0;
	for(int i=0;i<numPixels;i+=pixelSource.getBytesPerPixel()){
		r+=pixelSource[i];
		g+=pixelSource[i+1];
		b+=pixelSource[i+2];
	}
	r/=wXh;
	g/=wXh;
	b/=wXh;
	bgColor.set(r, g, b);
	update();
}

void ofxBluescreen::setBgColor(ofColor col)
{
	update();
}

void ofxBluescreen::setPixels(ofPixelsRef pixels)
{
	pixelSource.setFromPixels(pixels.getPixels(), pixels.getWidth(), pixels.getHeight(), OF_IMAGE_COLOR);//pixels.getBitsPerPixel());
	update();
}

void ofxBluescreen::setThreshhold(float thresh)
{
	threshhold = thresh;
	update();
}

void ofxBluescreen::update()
{
	//allocate(pixelSource.getWidth(), pixelSource.getHeight(), OF_IMAGE_COLOR_ALPHA);
	int wXh = pixelSource.getWidth() * pixelSource.getHeight();
	int numPixels = wXh * 4;
	unsigned char* pixels = new unsigned char[numPixels];
	ofColor c;
	int index=0;
	for(int i=0; i<wXh*3; i+=pixelSource.getBytesPerPixel()){
		c.set(pixelSource[i], pixelSource[i+1], pixelSource[i+2]);
		int alpha = 255;
		if(fabs(c.getHue()-bgColor.getHue())<threshhold)
			alpha=0;
		pixels[index] = c.r;
		pixels[index+1] = c.g;
		pixels[index+2] = c.b;
		pixels[index+3] = alpha;
		index+=4;
	}
	setFromPixels(pixels, pixelSource.getWidth(), pixelSource.getHeight(), OF_IMAGE_COLOR_ALPHA);
}

void ofxBluescreen::drawBgColor(int x, int y, int w, int h)
{
	ofFill();
	ofSetColor(bgColor);
	ofRect(x, y, w, h);
}

void ofxBluescreen::drawCheckers(int x, int y, int w, int h)
{
	int rectSize = 10;
	ofColor a(30);
	ofColor b(255);
	ofFill();
	int maxH=h/rectSize;
	int maxW=w/rectSize;
	for(int iy=0;iy<maxH;iy++){
		for(int ix=0;ix<maxW;ix++){
			if(iy%2==0)
				ix%2==0?ofSetColor(a):ofSetColor(b);
			else
				ix%2==0?ofSetColor(b):ofSetColor(a);
			ofRect(x+ix*rectSize, y+iy*rectSize, rectSize, rectSize);
		}
	}
}

void ofxBluescreen::draw(int x, int y, int w, int h, bool checkers)
{
	ofEnableAlphaBlending();
	if(checkers)
		drawCheckers(x, y, w, h);
	ofSetColor(255);
	ofImage::draw(x, y, w, h);
}

float ofxBluescreen::getThreshhold()
{
	return threshhold;
}