-- name: CreateWindSpeed :one
INSERT INTO wind_speed(id, created_at, updated_at, WindAngle, WindStrength, GustAngle, GustStrength)
VALUES (
    $1,
    NOW(),
    NOW(),
    $2,
    $3,
    $4,
    $5
)
RETURNING *;