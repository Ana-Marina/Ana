//Bulea Ana-Marina
//Tema EGC 2

#include "lab_blackbox.hpp"
#include "lab_camera.hpp"


//time
#include <ctime>


#define  KEY_ESCAPE		27

class Laborator5
	: public lab::glut::WindowListener
{
	private:
		// The BlackBox hides some functionality that we'll learn in the next course
		lab::BlackBox BLACKBOX;

		// Buffers used for holding state of keys
		// true - pressed
		// false - not pressed
		bool keyState[256];
		bool specialKeyState[256];

		// Objects
		lab::Mesh *player;
		std::vector<lab::Mesh*> scene;
		glm::vec3 player_pos; //pozitia jucatorului
		
		// Projection matrix
		bool isPerspectiveProjection;
		//matrice de proiectie perspectiva si ortogonala
		glm::mat4 projectionMatrix, projectionMatrix2;
		float FoV;
		float zNear, zFar;
		float aspectRatio;
		float orthoLeft, orthoRight, orthoTop, orthoBottom;

		float cameraRotation; //cat trebuie sa se roteasca camera
		float fallSpeed; //viteza de cadere a obiectului
		bool inAir; //daca jucatorul e in are sau sta pe un obiect

		// Camera
		lab::Camera cameraFPS, cameraTPS;

	public:
		Laborator5()
		{
			// init camera
			cameraFPS.set(glm::vec3(0, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0));
			cameraTPS.set(glm::vec3(0, 0, 40), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

			player_pos = glm::vec3(0, 0.2, 0);
			//mesh jucator
			player = load_Cube_as_Mesh(player_pos - glm::vec3(1), player_pos + glm::vec3(1), glm::vec3(1, 0, 0));

			//scena
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-2, -5, -2), glm::vec3(2, -1, 2), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-6, -5, -2), glm::vec3(-2, -1, 2), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-10, -5, -2), glm::vec3(-6, -1, 2), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-14, -5, -2), glm::vec3(-10, -1, 2), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-18, -5, -2), glm::vec3(-14, -1, 2), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-22, -5, -2), glm::vec3(-18, -1, 2), glm::vec3(0, 0, 0)));

			scene.push_back(load_Cube_as_Mesh(glm::vec3(-10, -1, -2), glm::vec3(-6, 3, 2), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-14, -1, -2), glm::vec3(-10, 3, 2), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-14, 3, -2), glm::vec3(-10, 7, 2), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-18, -1, -2), glm::vec3(-14, 3, 2), glm::vec3(0, 0, 0)));

			scene.push_back(load_Cube_as_Mesh(glm::vec3(-14, 3, -6), glm::vec3(-10, 7, -2), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-14, 3, -10), glm::vec3(-10, 7, -6), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-14, 3, -14), glm::vec3(-10, 7, -10), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-14, 3, -26), glm::vec3(-10, 7, -22), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-14, 3, -30), glm::vec3(-10, 7, -26), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-14, 3, -42), glm::vec3(-10, 7, -38), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-14, 3, -46), glm::vec3(-10, 7, -42), glm::vec3(0, 0, 0)));

			scene.push_back(load_Cube_as_Mesh(glm::vec3(-14, 7, -54), glm::vec3(-10, 11, -50), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-14, 7, -58), glm::vec3(-10, 11, -54), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-18, 7, -58), glm::vec3(-14, 11, -54), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-22, 7, -58), glm::vec3(-18, 11, -54), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-26, 7, -58), glm::vec3(-22, 11, -54), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-30, 7, -58), glm::vec3(-26, 11, -54), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-34, 7, -58), glm::vec3(-30, 11, -54), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-38, 7, -58), glm::vec3(-34, 11, -54), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-42, 7, -58), glm::vec3(-38, 11, -54), glm::vec3(0, 0, 0)));

			scene.push_back(load_Cube_as_Mesh(glm::vec3(-18, 7, -62), glm::vec3(-14, 11, -58), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-22, 7, -62), glm::vec3(-18, 11, -58), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-26, 7, -62), glm::vec3(-22, 11, -58), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-30, 7, -62), glm::vec3(-26, 11, -58), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-34, 7, -62), glm::vec3(-30, 11, -58), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-38, 7, -62), glm::vec3(-34, 11, -58), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-42, 7, -62), glm::vec3(-38, 11, -58), glm::vec3(0, 0, 0)));

			scene.push_back(load_Cube_as_Mesh(glm::vec3(-18, 11, -62), glm::vec3(-14, 15, -58), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-22, 11, -62), glm::vec3(-18, 15, -58), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-26, 11, -62), glm::vec3(-22, 15, -58), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-30, 11, -62), glm::vec3(-26, 15, -58), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-34, 11, -62), glm::vec3(-30, 15, -58), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-38, 11, -62), glm::vec3(-34, 15, -58), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-42, 11, -62), glm::vec3(-38, 15, -58), glm::vec3(0, 0, 0)));

			scene.push_back(load_Cube_as_Mesh(glm::vec3(-22, 15, -66), glm::vec3(-18, 19, -62), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-26, 15, -66), glm::vec3(-22, 19, -62), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-30, 15, -66), glm::vec3(-26, 19, -62), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-34, 15, -66), glm::vec3(-30, 19, -62), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-38, 15, -66), glm::vec3(-34, 19, -62), glm::vec3(0, 0, 0)));

			scene.push_back(load_Cube_as_Mesh(glm::vec3(-26, 19, -70), glm::vec3(-22, 23, -66), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-30, 19, -70), glm::vec3(-26, 23, -66), glm::vec3(0, 0, 0)));
			scene.push_back(load_Cube_as_Mesh(glm::vec3(-34, 19, -70), glm::vec3(-30, 23, -66), glm::vec3(0, 0, 0)));

			scene.push_back(load_Cube_as_Mesh(glm::vec3(-30, 23, -74), glm::vec3(-26, 27, -70), glm::vec3(0, 1, 0)));

			fallSpeed = 0;
			inAir = false;

			// initializa all key states to not pressed
			memset(keyState, 0, 256);
			memset(specialKeyState, 0, 256);

			// Initialize default projection values
			zNear = 0.1f;
			zFar = 500.0f;
			FoV = 60.0f;
			aspectRatio = 800 / 600.0f;
			orthoLeft = -25;
			orthoRight = 25;
			orthoBottom = -25;
			orthoTop = 25;
			cameraRotation = 0;

			// value may be used for updating the projection when reshaping the window
			isPerspectiveProjection = true;

			computePerspectiveProjection();
			computeOrthograhicProjection();
		}

		~Laborator5()
		{
			delete player;
			for (int i = 0; i < scene.size(); i++) {
				delete scene[i];
			}
		}

		lab::Mesh *load_Cube_as_Mesh(glm::vec3 cube1, glm::vec3 cube2, glm::vec3 color) {
			std::vector<lab::BlackBox::VertexFormat> vertecsi;
			std::vector<glm::uvec3> indecsi;

			vertecsi.push_back(lab::BlackBox::VertexFormat(cube1.x, cube1.y, cube1.z, color.x, color.y, color.z)); //0
			vertecsi.push_back(lab::BlackBox::VertexFormat(cube2.x, cube1.y, cube1.z, color.x, color.y, color.z)); //1
			vertecsi.push_back(lab::BlackBox::VertexFormat(cube2.x, cube1.y, cube2.z, color.x, color.y, color.z)); //2
			vertecsi.push_back(lab::BlackBox::VertexFormat(cube1.x, cube1.y, cube2.z, color.x, color.y, color.z)); //3

			vertecsi.push_back(lab::BlackBox::VertexFormat(cube1.x, cube2.y, cube1.z, color.x, color.y, color.z)); //4
			vertecsi.push_back(lab::BlackBox::VertexFormat(cube2.x, cube2.y, cube1.z, color.x, color.y, color.z)); //5
			vertecsi.push_back(lab::BlackBox::VertexFormat(cube2.x, cube2.y, cube2.z, color.x, color.y, color.z)); //6
			vertecsi.push_back(lab::BlackBox::VertexFormat(cube1.x, cube2.y, cube2.z, color.x, color.y, color.z)); //7

			indecsi.push_back(glm::uvec3(0, 1, 2));
			indecsi.push_back(glm::uvec3(0, 2, 3));
			indecsi.push_back(glm::uvec3(0, 1, 5));
			indecsi.push_back(glm::uvec3(0, 5, 4));
			indecsi.push_back(glm::uvec3(0, 3, 7));
			indecsi.push_back(glm::uvec3(0, 7, 4));

			indecsi.push_back(glm::uvec3(6, 4, 7));
			indecsi.push_back(glm::uvec3(6, 4, 5));
			indecsi.push_back(glm::uvec3(6, 7, 3));
			indecsi.push_back(glm::uvec3(6, 2, 3));
			indecsi.push_back(glm::uvec3(6, 5, 1));
			indecsi.push_back(glm::uvec3(6, 1, 2));
			
			unsigned int segments_vbo, segments_vao, segments_ibo;
			//creaza vao
			glGenVertexArrays(1, &segments_vao);
			glBindVertexArray(segments_vao);

			//creeaza vbo
			glGenBuffers(1, &segments_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, segments_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(lab::BlackBox::VertexFormat)*vertecsi.size(), &vertecsi[0], GL_STATIC_DRAW);

			//creeaza ibo
			glGenBuffers(1, &segments_ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, segments_ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3)*indecsi.size(), &indecsi[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(lab::BlackBox::VertexFormat), (void*)0);						//trimite pozitii pe pipe 0
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(lab::BlackBox::VertexFormat), (void*)(sizeof(float) * 3));		//trimite normale pe pipe 1

			//creez obiect de tip mesh
			lab::Mesh* mesh = new lab::Mesh(segments_vbo, segments_ibo, segments_vao, indecsi.size() * 3);
			mesh->cube[0] = cube1;
			mesh->cube[1] = cube2;

			return mesh;
		}

		//testeaza coliziunea obiectului cub cu jucatorul
		bool collision(lab::Mesh *cube) {
			//dimensiunile jucatorului si cubului
			float pwidth, pheight, pdepth, cwidth, cheight, cdepth;
			glm::vec3 cplayer, ccube; //centrul jucatorului si cubului
			
			pwidth = abs(player->cube[1].x - player->cube[0].x);
			pheight = abs(player->cube[1].y - player->cube[0].y);
			pdepth = abs(player->cube[1].z - player->cube[0].z);

			cwidth = abs(cube->cube[1].x - cube->cube[0].x);
			cheight = abs(cube->cube[1].y - cube->cube[0].y);
			cdepth = abs(cube->cube[1].z - cube->cube[0].z);

			cplayer = (player->cube[1] + player->cube[0]) / 2.0f + player_pos;
			ccube = (cube->cube[1] + cube->cube[0]) / 2.0f;

			if (abs(cplayer.x - ccube.x)*2 < pwidth + cwidth &&
				abs(cplayer.y - ccube.y)*2 < pheight + cheight &&
				abs(cplayer.z - ccube.z)*2 < pdepth + cdepth)
				return true;
			else return false;
		}

		// Function used to compute perspective projection matrix
		// use glm::perspective and member variables FoV, aspectRation, zNear and zFar
		void computePerspectiveProjection()
		{
			projectionMatrix = glm::perspective(FoV, aspectRatio, zNear, zFar);
		}

		void computeOrthograhicProjection()
		{
			projectionMatrix2 = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, zNear, zFar);
		}

		//---------------------------------------------------------------------
		// Loop Functions - function that are called every single frame

		// Called right before frame update callback (notifyDisplayFrame)
		void notifyBeginFrame() { };

		// Called every frame before we draw
		// Because glut sends only 1 key event every single frame, pressing more than 1 key will have no effect
		// If we treat the input using the 2 special buffers where we hold key states we can treat more than 1
		// key pressed at the same time. Also by not relling on glut to send the continuous pressing event
		// the moving motion will be more smooth because every single frame we apply the changes is contrast to
		// the event based method provided by Glut
		void treatInput()
		{
			// Compute frameRate independent speed for moving the camera
			// We set the speed based on the frameTime between the last 2 frames
			// Speed will remain constant no matter how many frames/second is the GPU cappable to render
			float frameTime = BLACKBOX.getFrameTimeSeconds();
			float moveSpeed = frameTime * 20;
			float movement = 0.0f; //miscarea jucatorului pe orizontala

			float rotateSpeedOX = frameTime * 40;
			float rotateSpeedOY = frameTime * 200;
			float rotateSpeedOZ = frameTime * 40;

			// Increase speed when Shift key is pressed
			if (specialKeyState[GLUT_KEY_SHIFT_L])
			{
				moveSpeed *= 2;
			}

			//salt
			if (keyState[' '] && !inAir) { fallSpeed = -35; inAir = true; }

			//miscare pe orizontala
			if (keyState['a']) movement -= moveSpeed;
			if (keyState['d']) movement += moveSpeed;

			//modificarea FOV
			if (keyState['+']) { FoV += 0.01; computePerspectiveProjection(); }
			if (keyState['-']) { FoV -= 0.01; computePerspectiveProjection(); }

			if (cameraRotation < 0) { //roteste scena la stanga
				//daca rotateSpeedOY e prea mare
				if (cameraRotation + rotateSpeedOY > 0) {
					cameraTPS.rotateTPS_OY(cameraRotation);
					cameraRotation = 0;
				}
				else {
					cameraTPS.rotateTPS_OY(-rotateSpeedOY);
					cameraRotation += rotateSpeedOY;
				}
			}
			else if (cameraRotation > 0) { //roteste scena la dreapta
				//daca rotateSpeedOY e prea mare
				if (cameraRotation - rotateSpeedOY < 0) {
					cameraTPS.rotateTPS_OY(cameraRotation);
					cameraRotation = 0;
				}
				else {
					cameraTPS.rotateTPS_OY(rotateSpeedOY);
					cameraRotation -= rotateSpeedOY;
				}
			}

			bool collided = false; //daca s-a gasit o coliziune
			glm::vec3 old_pos = player_pos;

			//aplica gravitatia
			player_pos -= cameraTPS.up * fallSpeed * frameTime;
			for (int i = 0; i < scene.size(); i++) {
				if (collision(scene[i])) {
					collided = true;
					break;
				}
			}

			if (collided) {
				//daca obiectul era in cadere si a colizat cu ceva nu mai e in aer
				if(fallSpeed > 0)
					inAir = false;

				fallSpeed = 0;
				player_pos = old_pos;
			}
			else if (frameTime < 1) { //vezi readme
				//translateaza camerele
				cameraTPS.translateUpword(-fallSpeed * frameTime);
				cameraFPS.translateUpword(-fallSpeed * frameTime);

				//acceleratie gravitationala
				fallSpeed += 100 * frameTime;

				inAir = true;
			}

			if (movement != 0) { //daca jucatorul se misca pe orizontala
				collided = false;
				old_pos = player_pos;

				//miscare relativa la pozitia camerei tps
				player_pos += cameraTPS.right * movement;
				for (int i = 0; i < scene.size(); i++) {
					if (collision(scene[i])) {
						collided = true;
						break;
					}
				}

				if (collided) {
					player_pos = old_pos;
				}
				else {
					//translateaza camerele
					cameraTPS.translateRight(movement);
					if (movement > 0)
						cameraFPS.set(player_pos, player_pos + cameraTPS.right, glm::vec3(0, 1, 0));
					else cameraFPS.set(player_pos, player_pos - cameraTPS.right, glm::vec3(0, 1, 0));
				}
			}
		}

		// A key was pressed
		void notifyKeyPressed(unsigned char key_pressed, int mouse_x, int mouse_y)
		{
			keyState[key_pressed] = 1;

			if (key_pressed == KEY_ESCAPE)
			{
				lab::glut::close();
			}

			if (key_pressed == 'o')
			{
				isPerspectiveProjection = false;
			}

			if (key_pressed == 'p')
			{
				isPerspectiveProjection = true;
			}
			// Reset Camera
			if (keyState['c'])
			{
				cameraFPS.set(glm::vec3(0, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0));
				cameraTPS.set(glm::vec3(0, 0, 40), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

				player_pos = glm::vec3(0, 0.2, 0);
				cameraRotation = 0;
				fallSpeed = 0;
				inAir = false;
			}

		}

		// When a key was released
		void notifyKeyReleased(unsigned char key_released, int mouse_x, int mouse_y)
		{
			keyState[key_released] = 0;

			// Disable drawing of camera target if no longer moving in ThirdPerson
			if (true)
			{
				char sum = 0;
				for (unsigned int i = 0; i <= 9; i++) {
					sum += keyState[i + '0'];
				}
			}

		}

		// Special key pressed like the navigation arrows or function keys F1, F2, ...
		void notifySpecialKeyPressed(int key_pressed, int mouse_x, int mouse_y)
		{
			specialKeyState[key_pressed] = 1;

			//rotatie scena
			if (key_pressed == GLUT_KEY_LEFT) {
				cameraRotation += 90;
			}

			if (key_pressed == GLUT_KEY_RIGHT) {
				cameraRotation -= 90;
			}

			switch (key_pressed)
			{
			case GLUT_KEY_F1: {
				lab::glut::enterFullscreen();
				break;
			}

			case GLUT_KEY_F2: {
				lab::glut::exitFullscreen();
				break;
			}

			case GLUT_KEY_F5: {
				BLACKBOX.LoadShader();
				break;
			}

			default:
				break;
			}
		}

		// Called when a special key was released
		void notifySpecialKeyReleased(int key_released, int mouse_x, int mouse_y)
		{
			specialKeyState[key_released] = 0;
		}

		// Called every frame to draw
		void notifyDisplayFrame()
		{
			// Treat continuous input
			treatInput();

			// Clear Color Buffer with the specified color
			glClearColor(1, 1, 1, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			BLACKBOX.notifyDisplay();

			// ----------------------------------
			// Set the viewport and view and projection matrices

			unsigned int width = lab::glut::getInitialWindowInformation().width;
			unsigned int height = lab::glut::getInitialWindowInformation().height;

			glViewport(0, 0, width, height);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			// Send view matrix to the GPU
			BLACKBOX.setViewMatrix(cameraTPS.getViewMatrix());
			
			// Send projection matrix to the GPU
			BLACKBOX.setProjectionMatrix(projectionMatrix2);

			BLACKBOX.setModelMatrix(glm::mat4(1));
			for (int i = 0; i < scene.size(); i++) {
				BLACKBOX.drawMesh(scene[i]);
			}

			BLACKBOX.setModelMatrix(glm::translate(glm::mat4(1), glm::vec3(player_pos)));
			BLACKBOX.drawMesh(player);

			//viewport secundar
			glViewport(0, 0, (float)width/4, (float)height/4);

			BLACKBOX.setViewMatrix(cameraFPS.getViewMatrix());

			if (isPerspectiveProjection)
				BLACKBOX.setProjectionMatrix(projectionMatrix);
			else
				BLACKBOX.setProjectionMatrix(projectionMatrix2);
			
			BLACKBOX.setModelMatrix(glm::mat4(1));
			for (int i = 0; i < scene.size(); i++) {
				BLACKBOX.drawMesh(scene[i]);
			}
		}

		// Called when the frame ended
		void notifyEndFrame() { }

		//---------------------------------------------------------------------
		// Function called when the windows was resized
		void notifyReshape(int width, int height, int previos_width, int previous_height)
		{
			//blackbox needs to know
			BLACKBOX.notifyReshape(width, height);
			aspectRatio = (float)width / height;

			orthoLeft = -(float)width / 30;
			orthoRight = (float)width / 30;
			orthoBottom = -(float)height / 30;
			orthoTop = (float)height / 30;

			computeOrthograhicProjection();
			computePerspectiveProjection();
		}

		//---------------------------------------------------------------------
		// Input function

		// Mouse drag, mouse button pressed 
		void notifyMouseDrag(int mouse_x, int mouse_y) { }

		// Mouse move without pressing any button
		void notifyMouseMove(int mouse_x, int mouse_y) { }

		// Mouse button click
		void notifyMouseClick(int button, int state, int mouse_x, int mouse_y) { }

		// Mouse scrolling
		void notifyMouseScroll(int wheel, int direction, int mouse_x, int mouse_y) { }
};

int main()
{
	// Initialize GLUT: window + input + OpenGL context
	lab::glut::WindowInfo window(std::string("Tema EGC 2"), 800, 600, 600, 100, true);
	lab::glut::ContextInfo context(3, 3, false);
	lab::glut::FramebufferInfo framebuffer(true, true, false, false);
	lab::glut::init(window, context, framebuffer);

	// Initialize GLEW + load OpenGL extensions 
	glewExperimental = true;
	glewInit();
	std::cout << "[GLEW] : initializare" << std::endl;

	// Create a new instance of Lab and listen for OpenGL callback
	// Must be created after GLEW because we need OpenGL extensions to be loaded

	Laborator5 *lab5 = new Laborator5();
	lab::glut::setListener(lab5);

	// Enter loop
	lab::glut::run();

	return 0;
}