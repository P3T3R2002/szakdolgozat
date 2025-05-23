// Code generated by sqlc. DO NOT EDIT.
// versions:
//   sqlc v1.27.0
// source: wind_data.sql

package database

import (
	"context"

	"github.com/google/uuid"
)

const createWindData = `-- name: CreateWindData :one
INSERT INTO wind_data(id, created_at, updated_at, wind_id, sensor_id)
VALUES (
    $1,
    NOW(),
    NOW(),
    $2,
    $3
)
RETURNING id, created_at, updated_at, sensor_id, wind_id
`

type CreateWindDataParams struct {
	ID       uuid.UUID
	WindID   uuid.UUID
	SensorID uuid.UUID
}

func (q *Queries) CreateWindData(ctx context.Context, arg CreateWindDataParams) (WindDatum, error) {
	row := q.db.QueryRowContext(ctx, createWindData, arg.ID, arg.WindID, arg.SensorID)
	var i WindDatum
	err := row.Scan(
		&i.ID,
		&i.CreatedAt,
		&i.UpdatedAt,
		&i.SensorID,
		&i.WindID,
	)
	return i, err
}
