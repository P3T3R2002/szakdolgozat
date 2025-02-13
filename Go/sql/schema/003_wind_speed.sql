-- +goose Up
CREATE TABLE wind_speed(
    id UUID primary key,
    created_at TIMESTAMP NOT NULL,
    updated_at TIMESTAMP NOT NULL,
    WindAngle INTEGER NOT NULL,
    WindStrength INTEGER NOT NULL,
    GustAngle INTEGER NOT NULL,
    GustStrength INTEGER NOT NULL
);

-- +goose Down
DROP TABLE orders;