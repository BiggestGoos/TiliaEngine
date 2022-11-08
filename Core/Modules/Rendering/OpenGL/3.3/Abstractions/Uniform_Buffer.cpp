// Vendor

// Standard

// Tilia

std::uint32_t tilia::gfx::Shader::s_bound_ID{};
std::uint32_t tilia::gfx::Shader::s_previous_ID{};

void tilia::gfx::Shader::Bind() const {
    if (!m_ID)
	{
		utils::Tilia_Exception e{ LOCATION };

		e.Add_Message("Failed to bind shader { ID: %v }"
		)(m_ID);

		throw e;

	}
    GL_CALL(glUseProgram(m_ID));
    s_bound_ID = m_ID;
}

void tilia::gfx::Shader::Bind(const std::uint32_t& id) {
    if (!id)
	{
		utils::Tilia_Exception e{ LOCATION };

		e.Add_Message("Failed to bind shader { ID: %v }"
		)(id);

		throw e;

	}
    GL_CALL(glUseProgram(id));
    s_bound_ID = id;
}

void tilia::gfx::Shader::Unbind(const bool& save_id) {
    GL_CALL(glUseProgram(0));

    if (save_id)
    {
        s_previous_ID = s_bound_ID;
    }

	s_bound_ID = 0;
}

void tilia::gfx::Shader::Rebind() {
	GL_CALL(glUseProgram(s_previous_ID));
	s_bound_ID = s_previous_ID;
	s_bound_ID = 0;
}