.PHONY: build
build:
	@docker build --target server -t brushknight/terrarium-server:latest -f server/Dockerfile server

.PHONY: publish
publish:
	@docker push brushknight/terrarium-server:latest
