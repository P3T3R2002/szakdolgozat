-- name: CreateWindData :one
INSERT INTO wind_data(id, created_at, updated_at, wind_id, sensor_id)
VALUES (
    $1,
    NOW(),
    NOW(),
    $2,
    $3
)
RETURNING *;