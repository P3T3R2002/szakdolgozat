// Code generated by sqlc. DO NOT EDIT.
// versions:
//   sqlc v1.27.0
// source: sensor_data.sql

package database

import (
	"context"

	"github.com/google/uuid"
)

const createSensor = `-- name: CreateSensor :one
INSERT INTO sensor_data(id, created_at, updated_at, Name, Long, Lat)
VALUES (
    $1,
    NOW(),
    NOW(),
    $2,
    $3,
    $4
)
RETURNING id, created_at, updated_at, name, long, lat
`

type CreateSensorParams struct {
	ID   uuid.UUID
	Name string
	Long float64
	Lat  float64
}

func (q *Queries) CreateSensor(ctx context.Context, arg CreateSensorParams) (SensorDatum, error) {
	row := q.db.QueryRowContext(ctx, createSensor,
		arg.ID,
		arg.Name,
		arg.Long,
		arg.Lat,
	)
	var i SensorDatum
	err := row.Scan(
		&i.ID,
		&i.CreatedAt,
		&i.UpdatedAt,
		&i.Name,
		&i.Long,
		&i.Lat,
	)
	return i, err
}
