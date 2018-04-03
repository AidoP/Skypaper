// Shader related utilities

#include <GL/gl.h>
#include <GL/glx.h>

static const char* defaultVertexShader =    "#version 330 core\n"
                                            "layout(location = 0) in vec3 pos;\n"
                                            "void main() {\n"
                                            "   gl_Position = vec4(pos.x, pos.y, 0.0F, 1.0F);\n"
                                            "}";

GLuint loadShaders(const char* vertexPath, const char* fragPath) {

    // Create shader ID's
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Load vertex shader
    char* vertexShaderCode;
    FILE* vertexFile = fopen(vertexPath, "r");
	if(vertexFile){
        // Get file size
        fseek(vertexFile, 0L, SEEK_END);
        long fileSize = ftell(vertexFile);
        rewind(vertexFile);

        // Allocate memory
        vertexShaderCode = calloc(1, fileSize + 1);
        if(!vertexShaderCode) {
            fclose(vertexFile);
            fatalError("Unable to allocate memory");
        }

        // Read file into memory
		if(1 != fread(vertexShaderCode, fileSize, 1, vertexFile)) {
            fclose(vertexFile);
            fatalError("Unable to read vertex shader");
        }
	} else{
        // Unable to open, use default
        if (vertexPath != 0)
            printf("Invalid vertex shader path. Using default.\n");
		vertexShaderCode = defaultVertexShader;

	}

    // Load fragment shader
    char* fragShaderCode;
    FILE* fragFile = fopen(fragPath, "r");
	if(fragFile){
        // Get file size
        fseek(fragFile, 0L, SEEK_END);
        long fileSize = ftell(fragFile);
        rewind(fragFile);

        // Allocate memory
        fragShaderCode = calloc(1, fileSize + 1);
        if(!fragShaderCode) {
            fclose(fragFile);
            fatalError("Unable to allocate memory");
        }

        // Read file into memory
		if(1 != fread(fragShaderCode, fileSize, 1, fragFile)) {
            fclose(fragFile);
            fatalError("Unable to read fragment shader");
        }
	} else
        fatalError("Unable to find fragment shader");
    fclose(fragFile);

    GLint result = GL_FALSE;
	int logLength;

    // Compile vertex shader
    printf("Compiling vertex shader\n");
	glShaderSource(vertexShaderID, 1, (const GLchar* const*) &vertexShaderCode , NULL);
	glCompileShader(vertexShaderID);

	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		char vertexShaderErrorMessage[logLength + 1];
        memset(vertexShaderErrorMessage, 0, (logLength + 1) * sizeof(char));
		glGetShaderInfoLog(vertexShaderID, logLength, NULL, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
        return 0;
	}


    // Compile fragment Shader
	printf("Compiling frag shader\n");
	glShaderSource(fragShaderID, 1, (const GLchar* const*) &fragShaderCode , NULL);
	glCompileShader(fragShaderID);

	glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragShaderID, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		char fragShaderErrorMessage[logLength + 1];
        memset(fragShaderErrorMessage, 0, (logLength + 1) * sizeof(char));
		glGetShaderInfoLog(fragShaderID, logLength, NULL, &fragShaderErrorMessage[0]);
		printf("%s\n", &fragShaderErrorMessage[0]);
        return 0;
	}


    // Link the shaders
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragShaderID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		char programErrorMessage[logLength + 1];
        memset(programErrorMessage, 0, (logLength + 1) * sizeof(char));
		glGetProgramInfoLog(programID, logLength, NULL, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage[0]);
        return 0;
	}

	
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragShaderID);
	
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragShaderID);

	return programID;
}