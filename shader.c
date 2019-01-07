/*
    shader - Open and interact with shaders
*/
#include "shader.h"

void read_file(const char* path, char** file_data) {

    // Allocate memory for the error return
    *file_data = malloc(strlen(SKYPAPER_FILE_OPEN_ERROR) + 1);
    if (!*file_data)
        fatal_error("Unable to allocate memory");

    // Open file for reading only
    FILE* file = fopen(path, "r");
    if (!file) {
        warning("Unable to open file");
        (*file_data)[0] = '\0';
        strcpy(*file_data, SKYPAPER_FILE_OPEN_ERROR);
        return;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long int file_size = ftell(file);
    rewind(file);


    // Reallocate memory for the read file data
    *file_data = realloc(*file_data, file_size + 1);
    if (!*file_data) {
        fclose(file);
        fatal_error("Unable to allocate memory");
    }
    (*file_data)[0] = '\0';

    // Read from the file
    int read_status = fread(*file_data, file_size, 1, file);
    if (read_status != 1) {
        warning("Unable to read file");

        *file_data = realloc(*file_data, strlen(SKYPAPER_FILE_READ_ERROR) + 1);
        if (!*file_data)
            fatal_error("Unable to allocate memory");
        (*file_data[0]) = '\0';
        strcpy(*file_data, SKYPAPER_FILE_READ_ERROR);
        return;
    }
    
    // Close file and return read data
    fclose(file);
}


void get_default_shader(GLenum shader_type, char** shader_source) {
    const char* shader_source_literal;
    if (shader_type == GL_FRAGMENT_SHADER) {
        shader_source_literal = SKYPAPER_DEFAULT_FRAGMENT_SHADER;
    } else if (shader_type == GL_VERTEX_SHADER) {
        shader_source_literal = SKYPAPER_DEFAULT_VERTEX_SHADER;
    } else {
        fatal_error("Invalid shader type passed to get_default_shader");
    }

    *shader_source = malloc(strlen(shader_source_literal) + 1);
    (*shader_source)[0] = '\0';
    strcpy(*shader_source, shader_source_literal);
}

GLuint compile_shader(GLenum shader_type, const char* path) {
    GLuint shader_object = glCreateShader(shader_type);
    // Make sure to free shader_source when done
    char* shader_source;
    if (!strcmp(path, "")) {
        read_file(path, &shader_source);

        // If there was an error reading the shaders, use the defaults
        if (strcmp(shader_source, SKYPAPER_FILE_OPEN_ERROR) || strcmp(shader_source, SKYPAPER_FILE_READ_ERROR)) {
            warning("Shader not found or unable to read. Using default");
            free(shader_source);
            get_default_shader(shader_type, &shader_source);
        }
    } else
        get_default_shader(shader_type, &shader_source);

    // Compile the shader
    glShaderSource(shader_object, 1, (const char**)&shader_source, NULL);
    glCompileShader(shader_object);

    free(shader_source);

    // Get log output
    GLint compile_status, error_message_length;
    glGetShaderiv(shader_object, GL_COMPILE_STATUS, &compile_status);
	glGetShaderiv(shader_object, GL_INFO_LOG_LENGTH, &error_message_length);
    if (compile_status != GL_TRUE) {
        char error_message[error_message_length + 1];
        glGetShaderInfoLog(shader_object, error_message_length, NULL, error_message);
        warning("Unable to compile shader: ");
        printf("------ For %s ------\n%s------------\n\n", path, error_message);
    }

    return shader_object;
}

GLuint load_shaders() {
    // Handle path strings
    char* shader_folder_path = "/shaders/";
    char* shader_path;
    char* fragment_shader_name = "frag.glsl";
    char* vertex_shader_name = "vertex.glsl";

    shader_path = malloc(strlen(shader_folder_path) + strlen(skypaper_settings.target_shader) + 2);
    shader_path[0] = '\0';
    strcat(shader_path, shader_folder_path);
    strcat(shader_path, skypaper_settings.target_shader);
    strcat(shader_path, "/");

    char* shader_directory = malloc(strlen(skypaper_settings.skypaper_data_directory) + strlen(shader_path) + 1);
    shader_directory[0] = '\0';
    char* fragment_shader_path = malloc(strlen(skypaper_settings.skypaper_data_directory) + strlen(shader_path) + strlen(fragment_shader_name) + 1);
    fragment_shader_path[0] = '\0';

    if (!shader_directory || !fragment_shader_path)
        fatal_error("Unable to allocate memory");

    /* Splice together path strings */
    {
        strcat(shader_directory, skypaper_settings.skypaper_data_directory);
        strcat(shader_directory, shader_path);

        strcat(fragment_shader_path, shader_directory);
        strcat(fragment_shader_path, fragment_shader_name);
    }

    char* vertex_shader_path = "";
    if (skypaper_settings.use_vertex_shader) {
        vertex_shader_path = malloc(strlen(skypaper_settings.skypaper_data_directory) + strlen(shader_path) + strlen(vertex_shader_name) + 1);
        if (!vertex_shader_path)
            fatal_error("Unable to allocate memory");
        vertex_shader_path[0] = '\0';
        strcat(vertex_shader_path, shader_directory);
        strcat(vertex_shader_path, vertex_shader_name);
    }

    // Ensure that our shader folder exists
    DIR* directory = opendir(shader_directory);
    if (!directory)
        warning("Shader folder ($XDG_DATA_HOME/skypaper/shaders) does not exist. Please see the readme");
    closedir(directory);

    // Compile the shaders into a shader program
    GLuint shader_program_object = glCreateProgram();
    glAttachShader(shader_program_object, compile_shader(GL_FRAGMENT_SHADER, fragment_shader_path));
    glAttachShader(shader_program_object, compile_shader(GL_VERTEX_SHADER, vertex_shader_path));

    // Bind output and link
    glBindFragDataLocation(shader_program_object, 0, "fragColor");
    glLinkProgram(shader_program_object);

    // Get log output
    GLint program_link_status, error_message_length;
    glGetProgramiv(shader_program_object, GL_LINK_STATUS, &program_link_status);
	glGetProgramiv(shader_program_object, GL_INFO_LOG_LENGTH, &error_message_length);
    if (program_link_status != GL_TRUE) {
        char error_message[error_message_length + 1];
        error_message[0] = '\0';
        glGetProgramInfoLog(shader_program_object, error_message_length, NULL, error_message);
        warning("Shader program linking failed:");
        printf("------------\n%s\n------------\n\n", error_message);
    }

    free(shader_path);
    free(shader_directory);
    free(fragment_shader_path);
    if(skypaper_settings.use_vertex_shader)
        free(vertex_shader_path);
    return shader_program_object;
}
