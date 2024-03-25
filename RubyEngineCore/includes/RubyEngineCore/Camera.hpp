#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace RubyEngine
{
	class Camera
	{
	public:
		enum class ProjectionMode
		{
			Perspective,
			Orthogonal
		};

		Camera(
			const glm::vec3& position = { 0, 0, 0 },
			const glm::vec3& rotation = { 0, 0, 0 },
			const ProjectionMode projection_mode = ProjectionMode::Perspective);

		void set_position(const glm::vec3& position);
		void set_rotation(const glm::vec3& rotation);
		void set_position_rotation(const glm::vec3& position, const glm::vec3& rotation);
		void set_projection_mode(const ProjectionMode projection_mode);

		const glm::mat4& get_view_matrix() const { return m_view_matrix; }
		const glm::mat4& get_projection_matrix() const { return m_projection_matrix; }

		void move_forward(const float delta);
		void move_right(const float delta);
		void move_up(const float delta);

		const glm::vec3& get_camera_position() { return m_position; }
		const glm::vec3& get_camera_rotation() { return m_rotation; }
		/*
		*	movement_delta.X - forward movement_delta.Y - right, movement_delta.Z - up
		*	rotation_delta.X - roll, rotation_delta.Y - pitch, rotation_delta.Z - yaw 
		*/
		void add_movement_and_rotation(const glm::vec3& movement_delta,
									   const glm::vec3 & rotation_delta);


	private:
		void update_view_matrix();
		void update_projection_matrix();

		ProjectionMode m_projection_mode;

		glm::vec3 m_position;
		glm::vec3 m_rotation; // X - roll, Y - pitch, Z - yaw

		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 m_direction;

		static constexpr glm::vec3 s_world_up		{ 0.f, 0.f, 1.f };
		static constexpr glm::vec3 s_world_right	{ 0.f, 1.f, 0.f };
		static constexpr glm::vec3 s_world_forward	{ 1.f, 0.f, 0.f };

		glm::mat4 m_view_matrix;
		glm::mat4 m_projection_matrix;
	};
}