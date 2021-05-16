.PHONY: build
build:
	@docker build --target server -t brushknight/terrarium-proxy:latest -f server/Dockerfile server

.PHONY: publish
publish:
	@docker push brushknight/terrarium-proxy:latest
