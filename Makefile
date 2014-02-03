all:
	@echo "> Building and placing in build/"
	@mkdir -p build/
	@g++ calculator.cpp -o build/calculator

clean:
	rm -rf build/
