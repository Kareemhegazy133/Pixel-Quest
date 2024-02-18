#include <simple_logger.h>

#include <pq_camera.h>

static pq_camera _camera = { 0 };

void pq_camera_set_position(Vector2D position)
{
	vector2d_copy(_camera.position, position);
	if (_camera.bind_camera)
	{
		pq_camera_apply_bounds();
	}
}

Vector2D pq_camera_get_position()
{
	return _camera.position;
}

void pq_camera_set_size(Vector2D size)
{
	vector2d_copy(_camera.size, size);
}

Vector2D pq_camera_get_offset()
{
	return vector2d(-_camera.position.x, -_camera.position.y);
}

void pq_camera_set_bounds(Rect bounds)
{
	gfc_rect_copy(_camera.bounds, bounds);
}

void pq_camera_enable_binding(Bool bind_camera)
{
	_camera.bind_camera = bind_camera;
}

void pq_camera_apply_bounds()
{
	if ((_camera.position.x + _camera.size.x) > (_camera.bounds.x + _camera.bounds.w))
	{
		_camera.position.x = (_camera.bounds.x + _camera.bounds.w) - _camera.size.x;
	}

	if ((_camera.position.y + _camera.size.y) > (_camera.bounds.y + _camera.bounds.h))
	{
		_camera.position.y = (_camera.bounds.y + _camera.bounds.h) - _camera.size.y;
	}

	if (_camera.position.x < _camera.bounds.x)
	{
		_camera.position.x = _camera.bounds.x;
	}

	if (_camera.position.y < _camera.bounds.y)
	{
		_camera.position.y = _camera.bounds.y;
	}
}

void pq_camera_center_on(Vector2D target)
{
	Vector2D position;
	position.x = target.x - (_camera.size.x * 0.5);
	position.y = target.y - (_camera.size.y * 0.5);
	pq_camera_set_position(position);

	if (_camera.bind_camera)
	{
		pq_camera_apply_bounds();
	}
}
