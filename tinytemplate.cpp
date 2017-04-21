// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "tinytemplate.hpp"
#include <stack>
#include <cassert>

tinytemplate::render_error::render_error(const std::string &what_arg)
    : std::runtime_error(what_arg)
{

}

tinytemplate::render_error::render_error(const char *what_arg)
    : std::runtime_error(what_arg)
{

}


struct RenderState {
    enum State
    {
        Output,
        Escape,
        Variable
    };
    State state;
    std::string output;

    RenderState(State _state)
        : state(_state)
    {

    }
};

void OutputState(std::string::const_iterator &sourceIter, std::stack<RenderState> &renderState, const std::map<std::string, std::string> &variables, bool ignoreErrors)
{
    switch(*sourceIter) {
    case '{':
        renderState.push(RenderState::Variable);
        break;
    case '\\':
        renderState.push(RenderState::Escape);
        break;
    case '}':
        if(renderState.size() > 1) {
            // We are inside variable expansion
            // current output names variable
            std::string varName = renderState.top().output;
            renderState.pop();
            auto varIter = variables.find(varName);
            if(varIter == variables.end()) {
                renderState.top().output.append("{{");
                renderState.top().output.append(varName);
                renderState.top().output.append("}}");
            }
            else {
                renderState.top().output.append(varIter->second);
            }
        }
        else {
            if(!ignoreErrors)
                throw tinytemplate::render_error("Unescaped close brace");
            renderState.top().output.push_back(*sourceIter);
        }
        break;
    default:
        renderState.top().output.push_back(*sourceIter);
    break;
    }
}

void VariableState(std::string::const_iterator &sourceIter, std::stack<RenderState> &renderState, bool ignoreErrors)
{
    switch (*sourceIter) {
    case '{':
        renderState.push(RenderState::Output);
        break;
    case '}':
    {
        // Copy current state
        RenderState variableState = renderState.top();
        renderState.pop();
        renderState.top().output.append(variableState.output);
    }
        break;
    default:
        if(!ignoreErrors)
            throw tinytemplate::render_error("Unescaped open brace");
        std::advance(sourceIter, -2);
        renderState.top().state = RenderState::Escape;
        break;
    }
}

void EscapeState(std::string::const_iterator &sourceIter, std::stack<RenderState> &renderState)
{
    renderState.pop();
    renderState.top().output.push_back(*sourceIter);
}

std::string tinytemplate::render(const std::string &source, const std::map<std::string, std::string> &variables, bool ignoreErrors)
{
    std::stack<RenderState> renderState;
    renderState.push(RenderState::Output);

    auto sourceIter = source.begin();
    while(sourceIter != source.end()) {
        switch (renderState.top().state) {
        case RenderState::Output:
            OutputState(sourceIter, renderState, variables, ignoreErrors);
            break;
        case RenderState::Escape:
            EscapeState(sourceIter, renderState);
            break;
        case RenderState::Variable:
            VariableState(sourceIter, renderState, ignoreErrors);
            break;
        default:
            assert("Unknown state");
            break;
        }

        std::advance(sourceIter, 1);
    }
    if(renderState.size() > 1) {
        if(!ignoreErrors)
            throw render_error("Unbalanced braces");
    }
    while(renderState.size() > 1) {
        std::string output = renderState.top().output;
        renderState.pop();
        renderState.top().output.append(output);
    }
    return renderState.top().output;
}
