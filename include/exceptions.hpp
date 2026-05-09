#pragma once

#include <string>
#include <exception>

// Matrix Exception Types
class NotInvertibleMatrixException: public std::exception {
private:
    std::string message;

public:
    explicit NotInvertibleMatrixException(std::string message) : message(message) {}

    const char* what() const noexcept override {
        return this->message.c_str();
    }
};

class InvalidFillTypeException: public std::exception {
private:
    std::string message;
public:
    explicit InvalidFillTypeException(std::string message) : message(message) {}

    const char* what() const noexcept override {
        return this->message.c_str();
    }
};

class IndexOutOfBoundException: public std::exception {
private:
    std::string message;
public:
    explicit IndexOutOfBoundException(std::string message) : message(message) {}

    const char* what() const noexcept override {
        return this->message.c_str();
    }
};

class MalformedMatrixException: public std::exception {
private:
    std::string message;
public:
    explicit MalformedMatrixException(std::string message) : message(message) {}

    const char* what() const noexcept override {
        return this->message.c_str();
    }
};

class ComputationFailedException: public std::exception {
private:
    std::string message;
public:
    explicit ComputationFailedException(std::string message) : message(message) {}

    const char* what() const noexcept override {
        return this->message.c_str();
    }
};

class InvalidArgumentException: public std::exception {
private:
    std::string message;
public:
    explicit InvalidArgumentException(std::string message) : message(message) {}

    const char* what() const noexcept override {
        return this->message.c_str();
    }
};
