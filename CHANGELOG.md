# Changelog

## [v1.4.1](https://github.com/fboulnois/pg_uuidv7/compare/v1.4.0...v1.4.1) - 2023-12-15

### Fixed

* Split into build and deploy stages

## [v1.4.0](https://github.com/fboulnois/pg_uuidv7/compare/v1.3.0...v1.4.0) - 2023-11-28

### Added

* Bump extension metadata to 1.4.0
* Run postgres with pg_uuidv7 extension

### Changed

* Move sql to top level to simplify install

## [v1.3.0](https://github.com/fboulnois/pg_uuidv7/compare/v1.2.0...v1.3.0) - 2023-09-19

### Added

* Bump extension metadata to 1.3.0
* Build extension for Postgres 13 and later
* Add Postgres 16 to test matrix
* Update Dockerfile to use Postgres 16
* Add Dockerfile to test and benchmark extension
* Add unit tests for pg_uuidv7

### Changed

* Add uuidv7 data sequentially
* Move extension sql into own directory

## [v1.2.0](https://github.com/fboulnois/pg_uuidv7/compare/v1.1.1...v1.2.0) - 2023-09-09

### Added

* Bump extension version to 1.2.0
* Build v7 uuid from timestamp

## [v1.1.1](https://github.com/fboulnois/pg_uuidv7/compare/v1.1.0...v1.1.1) - 2023-07-23

### Changed

* Simplify timestamp extraction

## [v1.1.0](https://github.com/fboulnois/pg_uuidv7/compare/v1.0.2...v1.1.0) - 2023-07-22

### Added

* Bump extension version to 1.1.0
* Extract timestamp from uuidv7

## [v1.0.2](https://github.com/fboulnois/pg_uuidv7/compare/v1.0.1...v1.0.2) - 2023-06-23

### Added

* Create tar.gz and checksums in container

### Changed

* Optimize copying of timestamp bits

## [v1.0.1](https://github.com/fboulnois/pg_uuidv7/compare/v1.0.0...v1.0.1) - 2023-04-29

### Added

* Bump to v1.0.1
* Automatically publish to pgxn
* Add json metadata for pgxn

### Fixed

* Execute publish to pgxn

## [v1.0.0](https://github.com/fboulnois/pg_uuidv7/releases/tag/v1.0.0) - 2023-01-02

### Added

* Add build using Dockerfile
* Add pg_uuidv7 extension to create v7 uuids
