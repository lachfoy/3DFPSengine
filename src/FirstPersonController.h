#pragma once

class Input;
class Camera;
class btKinematicCharacterController;

class FirstPersonController
{
public:
	FirstPersonController();
	~FirstPersonController();

	void HandleInput(Input* input);
	void Update(float dt);

	Camera* GetCamera();

private:
	Camera* m_camera;
	btKinematicCharacterController* m_characterController;

};
