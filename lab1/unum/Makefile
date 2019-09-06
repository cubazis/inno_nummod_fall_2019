CARGO = cargo

.PHONY: all build check clean test update

all: build

build:
	@$(CARGO) build --release

check:
	@$(CARGO) check

clean:
	@$(CARGO) clean

test: build
	@$(CARGO) test --release tests_posit

update:
	@$(CARGO) update
