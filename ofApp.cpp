#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofEnableDepthTest();

	auto radius = 15;
	auto x_span = radius * sqrt(3);
	auto flg = true;
	for (float y = -360; y < 360; y += radius * 1.5) {

		for (float x = -360; x < 360; x += x_span) {

			glm::vec3 location;
			if (flg) {

				location = glm::vec3(x, y, 0);
			}
			else {

				location = glm::vec3(x + (x_span / 2), y, 0);
			}

			this->location_list.push_back(location);
		}
		flg = !flg;
	}

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	for (int i = 0; i < this->location_list.size(); i++) {

		this->setHexagonToMesh(this->face, this->frame, this->location_list[i], 15, 5);
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	//ofRotateY(ofGetFrameNum() * 0.6666666666666);

	this->face.draw();
	this->frame.draw();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setHexagonToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float radius, float height) {

	ofColor face_color = ofColor(0);
	ofColor frame_color = ofColor(255);

	auto noise_param = ofNoise(glm::vec4(location * 0.004, ofGetFrameNum() * 0.005));
	glm::highp_mat4 rotation;
	if (noise_param < 0.3) {
	
		rotation = glm::rotate(glm::mat4(), ofMap(ofNoise(glm::vec4(location * 0.003, ofGetFrameNum() * 0.005)), 0, 1, -720, 720) * (float)DEG_TO_RAD * ofMap(noise_param, 0, 0.3, 0, 1), glm::vec3(-1, 1, 0));
	}
	else if (noise_param > 0.45 && noise_param < 0.55) {

		rotation = glm::rotate(glm::mat4(), ofMap(ofNoise(glm::vec4(location * 0.003, ofGetFrameNum() * 0.005)), 0, 1, -720, 720) * (float)DEG_TO_RAD * ofMap(noise_param, 0.45, 0.55, 0, 1), glm::vec3(0, 1, 0));
	}
	else if (noise_param > 0.7) {

		rotation = glm::rotate(glm::mat4(), ofMap(ofNoise(glm::vec4(location * 0.003, ofGetFrameNum() * 0.005)), 0, 1, -720, 720) * (float)DEG_TO_RAD * ofMap(noise_param, 0.7, 1, 0, 1), glm::vec3(1, 1, 0));
	}

	for (int deg = 90; deg < 450; deg += 60) {

		auto face_radius = radius - 0.5;
		auto face_index = face_target.getNumVertices();
		auto frame_index = frame_target.getNumVertices();

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3(0, 0, 0));
		vertices.push_back(glm::vec3(face_radius * cos(deg * DEG_TO_RAD), face_radius * sin(deg * DEG_TO_RAD), 0));
		vertices.push_back(glm::vec3(face_radius * cos((deg + 60) * DEG_TO_RAD), face_radius * sin((deg + 60) * DEG_TO_RAD), 0));
		vertices.push_back(glm::vec3(0, 0, height));
		vertices.push_back(glm::vec3(face_radius * cos((deg + 60) * DEG_TO_RAD), face_radius * sin((deg + 60) * DEG_TO_RAD), height));
		vertices.push_back(glm::vec3(face_radius * cos(deg * DEG_TO_RAD), face_radius * sin(deg * DEG_TO_RAD), height));

		for (auto& vertex : vertices) {

			vertex = glm::vec4(vertex, 0) * rotation;
			vertex += location;
		}

		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5);
		face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 4);
		face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5);

		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 1); frame_target.addIndex(frame_index + 2);
		frame_target.addIndex(frame_index + 4); frame_target.addIndex(frame_index + 5);
		frame_target.addIndex(frame_index + 1); frame_target.addIndex(frame_index + 5);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 4);

		for (int i = 0; i < vertices.size(); i++) {

			face_target.addColor(face_color);
			frame_target.addColor(frame_color);
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}