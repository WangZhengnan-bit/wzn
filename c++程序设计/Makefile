# 电子商务管理系统 Makefile
CXX      = g++
CXXFLAGS = -std=c++11 -Wall -g3
TARGET   = ecommerce.exe
OBJS     = main.o user.o product.o cart.o order.o store.o

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean run

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
