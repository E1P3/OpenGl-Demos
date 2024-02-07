#include "shader.h"
#include "entityModules/renderModule.h"
#include "lights.h"

	Shader::Shader() {}

	Shader::Shader(const char* PVS, const char* PFS, const char* PGS, const char* PTS, const char* TES) {
		this->Compile(this->readShaderSource(PVS), this->readShaderSource(PFS), this->readShaderSource(PGS), this->readShaderSource(PTS), this->readShaderSource(TES));
	}
	
	Shader& Shader::Use() {
		glUseProgram(ID);
		return *this;
	}

	unsigned int Shader::getID() const {
		return ID;
	}

	void Shader::setDebug(bool debug) {
        isDebug = debug;
    }

	void Shader::SetFloat(const std::string& name, float value, bool useShader) {
		if (useShader) {
			this->Use();
		}
		glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
		if (isDebug) {
			std::cout << "SetFloat: " << name << " = " << value << std::endl;
		}
	}

	void Shader::SetInteger(const std::string& name, int value, bool useShader) {
		if (useShader) {
			this->Use();
		}
		glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
		if (isDebug) {
			std::cout << "SetInteger: " << name << " = " << value << std::endl;
		}
	}

	void Shader::SetVector2f(const std::string& name, float x, float y, bool useShader) {
		if (useShader) {
			this->Use();
		}
		glUniform2f(glGetUniformLocation(this->ID, name.c_str()), x, y);
		if (isDebug) {
			std::cout << "SetVector2f: " << name << " = (" << x << ", " << y << ")" << std::endl;
		}
	}

	void Shader::SetVector2f(const std::string& name, const glm::vec2& value, bool useShader) {
		if (useShader) {
			this->Use();
		}
		glUniform2f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y);
		if (isDebug) {
			std::cout << "SetVector2f: " << name << " = (" << value.x << ", " << value.y << ")" << std::endl;
		}
	}

	void Shader::SetVector3f(const std::string& name, float x, float y, float z, bool useShader) {
		if (useShader) {
			this->Use();
		}
		glUniform3f(glGetUniformLocation(this->ID, name.c_str()), x, y, z);
		if (isDebug) {
			std::cout << "SetVector3f: " << name << " = (" << x << ", " << y << ", " << z << ")" << std::endl;
		}
	}

	void Shader::SetVector3f(const std::string& name, const glm::vec3& value, bool useShader) {
		if (useShader) {
			this->Use();
		}
		glUniform3f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z);
		if (isDebug) {
			std::cout << "SetVector3f: " << name << " = (" << value.x << ", " << value.y << ", " << value.z << ")" << std::endl;
		}
	}

	void Shader::SetVector4f(const std::string& name, float x, float y, float z, float w, bool useShader) {
		if (useShader) {
			this->Use();
		}
		glUniform4f(glGetUniformLocation(this->ID, name.c_str()), x, y, z, w);
		if (isDebug) {
			std::cout << "SetVector4f: " << name << " = (" << x << ", " << y << ", " << z << ", " << w << ")" << std::endl;
		}
	}

	void Shader::SetVector4f(const std::string& name, const glm::vec4& value, bool useShader) {
		if (useShader) {
			this->Use();
		}
		glUniform4f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z, value.w);
		if (isDebug) {
			std::cout << "SetVector4f: " << name << " = (" << value.x << ", " << value.y << ", " << value.z << ", " << value.w << ")" << std::endl;
		}
	}

	void Shader::SetMatrix4(const std::string& name, const glm::mat4& matrix, bool useShader) {
		if (useShader) {
			this->Use();
		}
		glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, false, glm::value_ptr(matrix));
		if (isDebug) {
			std::cout << "SetMatrix4: " << name << std::endl;

			for (int row = 0; row < 4; row++) {
				for (int col = 0; col < 4; col++) {
					std::cout << matrix[row][col] << " ";
				}
				std::cout << std::endl;
			}
		}
	}

	void Shader::Compile(const char* PVS, const char* PFS, const char* PGS, const char* PTS, const char* TES) {
		if (!PVS || !PFS) {
			#ifdef __APPLE__
				throw "SHADER_LINKING_ERROR: Shader data is empty";
			#else
				throw std::runtime_error("SHADER_LINKING_ERROR: Shader data is empty");
			#endif
		}

		ID = glCreateProgram();
		if (ID == 0)
		{
			fprintf(stderr, "Error creating shader program\n");
			exit(1);
		}

		unsigned int vertex, fragment, geometry, tessControl, tessEval;

		vertex = this->AddShader(PVS, GL_VERTEX_SHADER);
		fragment = this->AddShader(PFS, GL_FRAGMENT_SHADER);
		if (PGS) {
			geometry = this->AddShader(PGS, GL_GEOMETRY_SHADER);
		}
		if (PTS) {
			tessControl = this->AddShader(PTS, GL_TESS_CONTROL_SHADER);
		}
		if (TES) {
			tessEval = this->AddShader(TES, GL_TESS_EVALUATION_SHADER);
		}

		GLint Success = 0;
		GLchar ErrorLog[1024] = { 0 };

		// After compiling all shader objects and attaching them to the program, we can finally link it
		glLinkProgram(ID);
		glGetProgramiv(ID, GL_LINK_STATUS, &Success);
		if (Success == 0)
		{
			glGetProgramInfoLog(ID, sizeof(ErrorLog), NULL, ErrorLog);
			fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
			exit(1);
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if (PGS) {
			glDeleteShader(geometry);
		}
		if (PTS) {
			glDeleteShader(tessControl);
		}
		if (TES) {
			glDeleteShader(tessEval);
		}
	
	}

	void Shader::Delete()
	{
		glDeleteProgram(ID);
	}

	void Shader::Render(){}; // override in inherited class

	void Shader::bindRenderModule(RenderModule* object) {
		objectsToRender.push_back(object);
	}

	void Shader::bindDirectionalLight(DirectionalLight* light) {
		dirLightsToRender.push_back(light);
	}

	void Shader::bindPointLight(PointLight* light) {
		pointLightsToRender.push_back(light);
	}

	char* Shader::readShaderSource(const char* shaderFile) {
		FILE* fp;
		#ifdef _WIN32
			fopen_s(&fp, shaderFile, "rb");
		#else
			fp = fopen(shaderFile, "rb");
		#endif

		if (fp == NULL) { 
			std::cout << "Shader could not be loaded." << std::endl;	
			return NULL; 
		}

		fseek(fp, 0L, SEEK_END);
		long size = ftell(fp);

		fseek(fp, 0L, SEEK_SET);
		char* buf = new char[size + 1];
		fread(buf, 1, size, fp);
		buf[size] = '\0';

		fclose(fp);

		return buf;
	}

	unsigned int Shader::AddShader(const char* shaderText, GLenum shaderType)
	{
		// Create Shader Object and get its reference
		unsigned int ShaderObj = glCreateShader(shaderType);
		if (ShaderObj == 0)
		{
			fprintf(stderr, "Error creating shader type %d\n", ShaderObj);
			exit(0);
		}
		// Attach Shader source to the Vertex Shader Object
		glShaderSource(ShaderObj, 1, (const char**)&shaderText, NULL);
		// Compile the  Shader into machine code
		glCompileShader(ShaderObj);
		// Check for shader related errors using glGetShaderiv
		GLint success;
		glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char InfoLog[1024];
			glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
			fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, InfoLog);
			exit(1);
		}

		// Attach the Shader to the Shader Program
		glAttachShader(ID, ShaderObj);
		return ShaderObj;
	}