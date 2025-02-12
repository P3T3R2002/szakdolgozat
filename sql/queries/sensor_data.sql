-- name: CreateSensor :one
INSERT INTO sensor_data(id, created_at, updated_at, Name, Long, Lat)
VALUES (
    $1,
    NOW(),
    NOW(),
    $2,
    $3,
    $4
)
RETURNING *;