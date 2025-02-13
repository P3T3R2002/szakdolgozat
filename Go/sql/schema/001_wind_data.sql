-- +goose Up
CREATE TABLE wind_data(
    id UUID primary key,
    created_at TIMESTAMP NOT NULL,
    updated_at TIMESTAMP NOT NULL,
    sensor_id UUID NOT NULL REFERENCES sensor_data(id) ON DELETE CASCADE,
    wind_id UUID NOT NULL REFERENCES wind_speed(id) ON DELETE CASCADE
);

-- +goose Down
DROP TABLE orders;