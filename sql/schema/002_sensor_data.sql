-- +goose Up
CREATE TABLE sensor_data(
    id UUID primary key,
    created_at TIMESTAMP NOT NULL,
    updated_at TIMESTAMP NOT NULL,
    Name TEXT NOT NULL,
    Long FLOAT NOT NULL,
    Lat FLOAT NOT NULL
);

-- +goose Down
DROP TABLE orders;